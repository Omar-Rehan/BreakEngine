#include "pch.h"
#include "PoolAllocator.h"

PoolAllocator::PoolAllocator(int numOfNodes, size_t nodeSize, size_t alignment) {
	assert(!(alignment & (alignment - 1)) && "alignment is not a power of 2\n");
	assert(nodeSize >= sizeof(uintptr_t) && "node size is too small\n");
	Clear();

	m_totalSize = (numOfNodes * nodeSize) + alignment;
	m_pMemoryBlock = reinterpret_cast<uint8_t*>(new uint8_t[m_totalSize]);
	m_head = AlignPointer(reinterpret_cast<uintptr_t*>(m_pMemoryBlock), alignment);
	
	uintptr_t* ptr = m_head;
	for (int i = 0; i < numOfNodes - 1; i++) {
		uintptr_t* pNextNode = ptr + (nodeSize >> 2); // (int pointer + 1 == address + 4)
		*ptr = reinterpret_cast<uintptr_t>(pNextNode);
		ptr = pNextNode;
	}
	*ptr = reinterpret_cast<uintptr_t>(nullptr);
}
PoolAllocator::~PoolAllocator() {
	Clear();
}

void* PoolAllocator::Allocate() {
	uintptr_t* crntNode = m_head;
	uintptr_t* nextNode = reinterpret_cast<uintptr_t*>(*crntNode);
	m_head = nextNode;
	return crntNode;
}
void PoolAllocator::Deallocate(void* pMemory) {
	assert(m_pMemoryBlock <= pMemory && pMemory < m_pMemoryBlock + m_totalSize && "trying to deallocate an invalid pointer");

	uintptr_t* nextNode = m_head;
	uintptr_t* crntNode = reinterpret_cast<uintptr_t*>(pMemory);
	*crntNode = reinterpret_cast<uintptr_t>(nextNode);
	m_head = crntNode;
}
void PoolAllocator::Clear() {
	delete[] m_pMemoryBlock;
	m_pMemoryBlock = nullptr;
	m_head = nullptr;
}

/// For debugging purposes
void PoolAllocator::PrintAddresses() {
	uintptr_t* ptr = m_head;
	while (ptr != nullptr) {
		std::cout << ptr << " ";
		ptr = reinterpret_cast<uintptr_t*>(*ptr);
	}
	std::cout << std::endl;
}