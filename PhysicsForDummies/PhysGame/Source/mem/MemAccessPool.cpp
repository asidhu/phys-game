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
}

void MemAccessPool::addToFreeList(MemChunk* chunk){
	//this block is out of place, add it in free space.
	MemChunk* it = m_rootFree;
	MemChunk* prev = NULL;
	while (it->nextFree != NULL){
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
		it = it->nextFree;
	}
	it->nextFree = chunk;
	chunk->nextFree = NULL;
}


void* MemAccessPool::allocate(int sz){
	MemChunk* it = m_rootFree;
	MemChunk* prev = NULL;
	while (it != NULL){
		if (it->size > sz){//size is bigger lets allocate it!
			//check space left in block
			int remainingSpace = it->size-sz;
			// if remaining space > minimum chunk size allocate OR if there is no other free block (UHOH LOW MEMORY or BUG)
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
				assert(it->next == NULL || it->next->used);
				it->nextUsed = it->next;
				if (it->prev){
					assert(it->prev->used);
					it->prev->nextUsed = it;
				}
				else{
					m_rootUsed = it;//no previous so must be at root.
				}

				//fix freed list
				if (prev == NULL){//root free node, automatically smallest block
					m_rootFree = newBlock;
					newBlock->nextFree = nxtFree;
				}
				else{
					//need to maintain sorted order, remove old free block
					prev->nextFree = nxtFree;
					//search through free list for correct spot.
					addToFreeList(newBlock);
				}
				//setup in memory list
				if (it->next != NULL)
					it->next->prev = newBlock;
				newBlock->next = it->next;
				newBlock->prev = it;
				it->next = newBlock;
				return it->data();
			}
			//we have no choice... may as well use this last block... its big enough dont bother making a new mem block tho.
			if (it->nextFree == NULL){
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
				//We know there are no more free blocks. If we reached this branc
				if (prev == NULL)
					m_rootFree = NULL;
				else
					prev->nextFree = NULL;

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

void MemAccessPool::deallocate(void* data){
	assert(data>m_dataBlock);
	assert(((uintptr_t)data - (uintptr_t)m_dataBlock)<size);//bounds check on deletions
	MemChunk* chunk = (MemChunk*)((uintptr_t)data - sizeof(MemChunk));	//best thing ever.
	assert(chunk->data() == data);
	assert(chunk->used);//make sure its currently used
	assert(chunk->size);//make sure its size >0 SANITY CHECK.
	bool combineBack = chunk->prev != NULL && !chunk->prev->used;//check if we have to merge block behind us
	bool combineForward = chunk->next != NULL && !chunk->next->used;//check if we have to merge block in front of us
	freeSpace += chunk->used + sizeof(MemChunk);
	chunk->used = 0;

	if (combineForward){
		//new size of chunk is current size next chunks data portion and the size of header
		chunk->size += chunk->next->size + sizeof(MemChunk);
		//fix in mem list (remove chunk->next from list)
		if(chunk->next->next)
			chunk->next->next->prev = chunk;
		chunk->next = chunk->next->next;


	}
	if (combineBack){
		chunk = chunk->prev;
		chunk->size += chunk->next->size + sizeof(MemChunk);
		//fix in mem list (remove chunk->next from list)
		if (chunk->next->next)
			chunk->next->next->prev = chunk;
		chunk->next = chunk->next->next;
	}

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
		//fix freed list! :O have to do a free node search unfortunately :(
		addToFreeList(chunk);
	

}




MemAccessPool::~MemAccessPool(){
	free(m_dataBlock);//CLEANING DONE... :D
}