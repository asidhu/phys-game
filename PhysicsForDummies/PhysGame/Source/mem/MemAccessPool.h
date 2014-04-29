#pragma once
/*
Purpose of class, Memory pool allocator with triple linked list optimized for linear iteration, creation, and deletion

min allocation size is so tiny memory blocks arent created wasting space. This does mean that some memory is wasted 

Free list is sorted in ascending block size order for speedy creation.
Used list is sorted in-memory order for highest cache hit rate during linear iteration(but skips over free blocks)
Base list is in-memory order for speedy deletion.

*/

class MemAccessPool{
	void* m_dataBlock;
	MemChunk * m_root;
	MemChunk * m_rootUsed;
	MemChunk * m_rootFree;
	int freeSpace;
	int size;
	int minimumFreeChunkSize;
public:
	MemAccessPool(int maxSize,int minAllocationSize=8);
	~MemAccessPool();
	void* allocate(int size);
};

class MemChunk{
public:
	void* data;
	int used;
	int size;
	MemChunk* next;
	MemChunk* nextUsed;
	MemChunk* nextFree;

};