#include "MemAccessPool.h"
#include <cstdlib>
MemAccessPool::MemAccessPool(int initialSize, int minAllocSize){
	m_dataBlock = malloc(initialSize);
	m_dataBlock_end = (void*)((uintptr_t)m_dataBlock + initialSize);
	assert(m_dataBlock != NULL);
	freeSpace =initialSize - sizeof(MemChunk);
	m_root = (MemChunk*)m_dataBlock;
	m_rootUsed = NULL;
	m_root->size = freeSpace;
	m_root->used = 0;
	m_root->next = NULL;
	m_root->prev = NULL;
	m_root->nextUsed = NULL;
	m_root->nextFree = NULL;
	m_rootFree = m_root;
	size = initialSize;
	minimumFreeChunkSize = minAllocSize + sizeof(MemChunk);
	numFree = 1;
	numAllocated = 0;
}

void MemAccessPool::removeFromFreeList(MemChunk* chunk){
	//need to remove arbitrary memory chunk.
	assert(!chunk->used);
	MemChunk* it = m_rootFree;
	MemChunk* prev = NULL;
	while (it){
		if (it == chunk){
			if (!prev){
				m_rootFree = chunk->nextFree;
			}
			else{
				prev->nextFree = chunk->nextFree;
			}
			break;
		}
		prev = it;
		it = it->nextFree;
	}
}
void MemAccessPool::addToFreeList(MemChunk* chunk){
	//this block is not in free list, lets add it
	if (!m_rootFree)
	{
		m_rootFree = chunk;
		return;
	}
	MemChunk* it = m_rootFree;
	MemChunk* prev = NULL;
	while (it){
		if (it->size >= chunk->size){
			if (prev == NULL){
				chunk->nextFree = it;
				m_rootFree = chunk;
			}
			else{
				chunk->nextFree = it;
				prev->nextFree = chunk;
			}
			return;
		}
		prev = it;
		it = it->nextFree;
	}
	prev->nextFree = chunk;
	chunk->nextFree = NULL;
}


void* MemAccessPool::allocate(int sz){
	MemChunk* it = m_rootFree;
	MemChunk* prev = 0L;
	while (it){
		if (it->size == sz){//size is equal!
				//decrement free space
			freeSpace -= sz + sizeof(MemChunk);
			assert(freeSpace > 0);
			it->used = sz;
			//in memory list is the same.
			//used memory list update
			MemChunk* tmp = it->nextFree;

			it->nextUsed = it->next;
			if (it->prev){
				assert(it->prev->used);
				it->prev->nextUsed = it;
			}
			else{
				assert(m_rootUsed == it->next);
				m_rootUsed = it;
			}

			if (!prev){
				//set root free to the next free element
				m_rootFree = tmp;
			}
			else{
				//remove it from the free list
				prev->nextFree = tmp;
			}
			numFree--;
			numAllocated++;
			return it->data();
		}
		if (it->size > sz){//size is bigger lets allocate it!
			//check space left in block
			int remainingSpace = it->size-sz;
			// if remaining space > minimum chunk size allocate
			if (remainingSpace > minimumFreeChunkSize ){
				//decrement free space
				freeSpace -= sz+ sizeof(MemChunk);
				assert(freeSpace > 0);
				//create new memory block, offset by header and size of data
				MemChunk* newBlock = (MemChunk*)((uintptr_t)it + sizeof(MemChunk)+sz);
				newBlock->used = 0;
				//size is size of initial block - size of data - size of new header
				newBlock->size = it->size - sz - sizeof(MemChunk);
				//shrink old block
				it->size = sz;
				it->used = sz;
				MemChunk* nxtFree = it->nextFree;
				//fix used list
				//next and prev block in memory must be "used" or free algorithm would merge the blocks
				assert(!it->next || it->next->used);
				it->nextUsed = it->next;
				if (it->prev){
					assert(it->prev->used);
					it->prev->nextUsed = it;
				}
				else{
					assert(m_rootUsed == it->next);
					m_rootUsed = it;//no previous so must be at root.
				}
				//setup in memory list
				if (it->next)
					it->next->prev = newBlock;
				newBlock->next = it->next;
				newBlock->prev = it;
				it->next = newBlock;
				//fix freed list
				if (!prev){//root free node, automatically smallest block
					m_rootFree = newBlock;
					newBlock->nextFree = nxtFree;
				}
				else{
					//need to maintain list order, remove old free block
					prev->nextFree = nxtFree;
					//search through free list for correct spot.
					addToFreeList(newBlock);
				}
				numAllocated++;
				return it->data();
			}
			//we have no choice... may as well use this last block... its big enough dont bother making a new mem block tho.
			if (!it->nextFree){
				freeSpace -= sz;
				assert(freeSpace > 0);
				//uh oh... dont create a new chunk.
				//shrink old block
				it->used = sz;
				assert(it->next->used);
				//fixed used list
				it->nextUsed = it->next;
				if (it->prev){
					assert(it->prev->used);
					it->prev->nextUsed = it;
				}
				//fixed free list
				//We know there are no more free blocks. If we reached this branch
				if (prev == NULL)
					m_rootFree = NULL;
				else
					prev->nextFree = NULL;

				numFree--;
				numAllocated++;
				//in memory list, no new chunks are created so no changes.
				return it->data();
			}

		}
		prev = it;
		it = it->nextFree;
	}
	//no free block big enough :( sorry? maybe allocate some more memory here.
	return NULL;
}
bool MemAccessPool::own(void* data){
	return data > m_dataBlock && ((uintptr_t)data - (uintptr_t)m_dataBlock)<size;
}

void MemAccessPool::innerFree(MemChunk* chunk){
	assert(chunk);
	assert(chunk->used);//make sure its currently used
	assert(chunk->size);//make sure its size >0 SANITY CHECK.
	bool combineBack = chunk->prev != NULL && !chunk->prev->used;//check if we have to merge block behind us
	bool combineForward = chunk->next != NULL && !chunk->next->used;//check if we have to merge block in front of us
	freeSpace += chunk->used + sizeof(MemChunk);

	if (combineForward){

		//remove this chunk from free list because it needs to be readded.
		removeFromFreeList(chunk->next);
		//new size of chunk is current size next chunks data portion and the size of header
		chunk->size += chunk->next->size + sizeof(MemChunk);
		//fix in mem list (remove chunk->next from list)
		if (chunk->next->next)
			chunk->next->next->prev = chunk;
		chunk->next = chunk->next->next;
		numFree--;
	}
	if (combineBack){
		//remove from free list to be readded.
		removeFromFreeList(chunk->prev);
		chunk = chunk->prev;
		chunk->size += chunk->next->size + sizeof(MemChunk);
		//fix in mem list (remove chunk->next from list)
		if (chunk->next->next)
			chunk->next->next->prev = chunk;
		chunk->next = chunk->next->next;
		numFree--;
	}
	chunk->used = 0;
	assert(!chunk->prev || chunk->prev->used);
	assert(!chunk->next || chunk->next->used);
	//in mem list stays same.
	//change used list. Prev must be used.
	if (!chunk->prev)
	{
		//we are at root :O!!!!! set root used to next used = next
		m_rootUsed = chunk->next;
	}
	else{
		//next used of previous must be next.
		chunk->prev->nextUsed = chunk->next;
	}
	numFree++;
	numAllocated--;
	//fix freed list! :O have to do a free node search unfortunately :(
	addToFreeList(chunk);
}

void MemAccessPool::deallocate(void* data){
	assert(own(data));//bounds check on deletions
	MemChunk* chunk = (MemChunk*)((uintptr_t)data - sizeof(MemChunk));	//best thing ever.
	assert(chunk->data() == data);
	innerFree(chunk);
}




MemAccessPool::~MemAccessPool(){
	free(m_dataBlock);//CLEANING DONE...  <--- also my favoritest line in this file.
}

void MemAccessPool::validateLists(){
	//validate free list 
	MemChunk* it;
	//validate in memory list
	it = m_root;
	MemChunk* prev = NULL;
	assert(m_root == m_dataBlock);
	int size = 0;
	while (it){
		assert(it->prev == prev);
		assert(!it->next || it->next == (MemChunk*)((uintptr_t)it + it->size + sizeof(MemChunk)));
		prev = it;
		it = it->next;
		size++;
	}
	assert(size == numFree + numAllocated);
	size = 0;
	it = m_rootFree;
	while (it){
		assert(!it->used);
		assert(!it->next || it->next->used);
		assert(!it->nextFree || it->nextFree->size >= it->size);//ascending order check
		it = it->nextFree;
		size++;
	}
	assert(numFree == size);
	size = 0;
	//validate used list
	it = m_rootUsed;
	while (it){
		assert(it->used);
		it = it->nextUsed;
		size++;
	}
	assert(numAllocated == size);


}


