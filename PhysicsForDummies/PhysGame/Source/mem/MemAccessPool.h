#pragma once
#include <cassert>
#include <iterator>
/*
Purpose of class, Memory pool allocator with triple linked list optimized for linear iteration, creation, and deletion

min allocation size is so tiny memory blocks arent created wasting space. This does mean that some memory is wasted 

doesn't support resize :O that would be inefficient cache wise. could be added tho.

Free list is sorted in ascending block size order for speedy creation.
Used list is sorted in-memory order for highest cache hit rate during linear iteration(but skips over free blocks)
Base list is in-memory order for speedy deletion.

*/

class MemChunk{
public:
	int used;
	int size;
	MemChunk *next, *prev;
	//Syntax sugar, can only be in one list at a time, saves header space
	union{
		MemChunk* nextUsed;
		MemChunk* nextFree;
	};
	void* data(){ return (void*)((uintptr_t)this + sizeof(MemChunk)); }
};
class MemAccessPool{
	void* m_dataBlock, *m_dataBlock_end;
	MemChunk * m_root;
	MemChunk * m_rootUsed;
	MemChunk * m_rootFree;
	int freeSpace;
	int size;
	int numFree;
	int numAllocated;
	int minimumFreeChunkSize;
	void addToFreeList(MemChunk* chunk);
	void removeFromFreeList(MemChunk* chunk);
	void innerFree(MemChunk* chunk);
	void validateLists();
public:
	MemAccessPool(int maxSize,int minAllocationSize=8);
	~MemAccessPool();
	void* allocate(int size);
	void  deallocate(void* data);
	int getFreeSpace(){ return freeSpace; }
	bool own(void*);
	class iterator{
	public:
		typedef iterator self_type;
		typedef std::forward_iterator_tag iterator_category;
		typedef int difference_type;
		iterator(MemChunk* first) : it(first) { }
		self_type operator++() { assert(it != NULL); it = it->nextUsed; return *this; }
		self_type operator++(int junk) { assert(it != NULL); it = it->nextUsed; return *this; }
		void* operator*() { assert(it != NULL); return it->data(); }
		void* operator->() { assert(it != NULL); return it; }
		bool operator==(const self_type& rhs) { return it  == rhs.it; }
		bool operator!=(const self_type& rhs) { return it != rhs.it; }
	private:
		MemChunk* it;
		friend class MemAccessPool;
	};

	iterator begin(){
		return iterator(m_rootUsed);
	}
	iterator end(){
		return iterator(NULL);
	}

	iterator& erase(iterator& it){
		MemChunk* freed = it.it;
		assert(freed);
		it++;
		innerFree(freed);
		return it;
	}

	template<class T>
	T* allocate(){
		void* data = allocate(sizeof(T));
		return (T*)(data);
	}

	template<class T, typename... ConstructorArgs>
	T* allocate(ConstructorArgs... args){
		void* data = allocate(sizeof(T));
		if (!data)
			return NULL;
		return new (data)T( args... );
	}
};
