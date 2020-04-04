#include "pch.h"
#include "StackAllocator.h"

StackAllocator::StackAllocator(size_t totalBytes) {
	Clear();
	m_pMemoryBlock = new uint8_t[totalBytes];
	m_allocatedObjects.Push(m_pMemoryBlock + totalBytes);
}
StackAllocator::~StackAllocator() {
	Clear();
}

void* StackAllocator::Allocate(size_t neededBytes, size_t alignment) {
	assert(!(alignment & (alignment - 1)) && "alignment is not a power of 2\n");

	size_t actualBytes = neededBytes + alignment;
	uint8_t* pRawMemory = reinterpret_cast<uint8_t*>(MakeSpace(actualBytes));
	
	uint8_t* pAlignedMemory = AlignPointer(pRawMemory, alignment);
	if (pAlignedMemory == pRawMemory) pAlignedMemory += alignment;

	ptrdiff_t shift = pAlignedMemory - pRawMemory;
	assert(0 < shift && shift <= 256);
	
	pAlignedMemory[-1] = static_cast<uint8_t>(shift & 0xff);
	return pAlignedMemory;
}
void StackAllocator::Deallocate(void* pMemory) {
	assert(pMemory && "Trying to deallocate a null pointer\n");

	uint8_t* pAlignedMemory = reinterpret_cast<uint8_t*>(pMemory);
	ptrdiff_t shift = pAlignedMemory[-1];
	if (!shift) shift = 256;

	uint8_t* pRawMemory = pAlignedMemory - shift;
	FreeSpace(pRawMemory);
}
void StackAllocator::PrintAddresses() {
	m_allocatedObjects.Print();
}

void* StackAllocator::MakeSpace(size_t bytes) {
	assert(m_allocatedObjects.Peek() - bytes >= m_pMemoryBlock && "Not enough space for the stack allocator\n");

	m_allocatedObjects.Push(m_allocatedObjects.Peek() - bytes);
	return m_allocatedObjects.Peek();
}
void StackAllocator::FreeSpace(uint8_t* pMemory) {
	assert(pMemory == m_allocatedObjects.Peek() && "Trying to deallocate memory not on top of the stack\n");
	m_allocatedObjects.Pop();
}
void StackAllocator::Clear() {
	delete[] m_pMemoryBlock;
	m_pMemoryBlock = nullptr;
	m_allocatedObjects.Clear();
}