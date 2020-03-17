#pragma once
#include "Allocator.h"

class PoolAllocator : public Allocator {
public:
	PoolAllocator(int numOfNodes, size_t nodeSize, size_t alignment);
	~PoolAllocator();

	void* Allocate();
	virtual void Deallocate(void* pMemory);
	virtual void Clear();
	virtual void PrintAddresses();

private:
	uintptr_t* m_head = nullptr;
	size_t m_totalSize = NULL;

	PoolAllocator(const PoolAllocator&) = delete;
	PoolAllocator(PoolAllocator&&) = delete;
	PoolAllocator operator = (const PoolAllocator&) = delete;
	PoolAllocator operator = (PoolAllocator&&) = delete;
};