#pragma once
#include <cstdint>
#include <assert.h>
#include <iostream>

class Allocator {
public:
	virtual void Deallocate(void* pMemory) = 0;
	virtual void PrintAddresses() = 0;

protected:
	uint8_t* m_pMemoryBlock = nullptr;
	virtual void Clear() = 0;

	template <typename T>
	inline T* AlignPointer(T* ptr, size_t align) {
		const uintptr_t address = reinterpret_cast<uintptr_t>(ptr);
		const size_t mask = align - 1;
		return reinterpret_cast<T*>((address + mask) & ~mask);
	}
};