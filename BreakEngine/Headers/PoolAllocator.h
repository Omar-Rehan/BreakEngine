#pragma once
#include <cstdint>

class PoolAllocator {
public:
	PoolAllocator() = default;
	~PoolAllocator() = default;

	void Initialize(int numOfNodes, size_t nodeSize, size_t alignment);
	void Terminate();
	void* Allocate();
	void Deallocate(void* pMemory);
	void Clear();
	void PrintAddresses();

private:
	size_t m_totalSize = NULL;
	uintptr_t* m_head = nullptr;
	uint8_t* m_pMemoryBlock = nullptr;

	template <typename T>
	inline T* AlignPointer(T* ptr, size_t align) {
		const uintptr_t address = reinterpret_cast<uintptr_t>(ptr);
		const size_t mask = align - 1;
		return reinterpret_cast<T*>((address + mask) & ~mask);
	}


	PoolAllocator(const PoolAllocator&) = delete;
	PoolAllocator(PoolAllocator&&) = delete;
	PoolAllocator operator = (const PoolAllocator&) = delete;
	PoolAllocator operator = (PoolAllocator&&) = delete;
};