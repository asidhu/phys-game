#include "MemAccessPool.h"
#include <cstdlib>

MemAccessPool::MemAccessPool(int initialSize, int minAllocSize){
	m_dataBlock = malloc(initialSize);
	m_root = (MemChunk*)m_dataBlock;
	m_rootUsed = NULL;
	m_root->data = m_root + sizeof(MemChunk);
	m_root->size = m_root->used = 0;
	m_root->next = NULL;
	m_root->nextUsed = NULL;
	m_root->nextFree = NULL;
	m_rootFree = m_root;
	freeSpace -= sizeof(MemChunk);
	size = initialSize;
	minimumFreeChunkSize = minAllocSize + sizeof(MemChunk);
}

void* MemAccessPool::allocate(int sz){
	MemChunk* it = m_rootFree;
	while (it != NULL){
		if (it->size > sz){//size is bigger lets allocate it!
			//check space left in block
			int remainingSpace = it->size-sz;
			it->size -= sz;

			//decrement free space
			freeSpace -= sz;



		}
		it = it->nextFree;
	}
	
}

MemAccessPool::~MemAccessPool(){
	free(m_dataBlock);//CLEANING DONE...
}