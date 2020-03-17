#pragma once
#include "Allocator.h"

class StackAllocator : public Allocator {
private:
	struct Stack {
	private:
		int topIndex = -1;
		uint8_t* elements[200] = {nullptr}; // TODO: is 200 objects in a stack memory enough?

	public:
		void Push(uint8_t* object) {
			assert(topIndex < 199 && "Too many objects in the stack\n");
			elements[++topIndex] = object;
		}
		uint8_t* Pop() {
			assert(topIndex > -1 && "No objects in the stack\n");
			return elements[topIndex--];
		}
		uint8_t* Peek() {
			assert(topIndex > -1 && "No objects in the stack\n");
			return elements[topIndex];
		}
		void Clear() {
			topIndex = -1;
		}
		void Print() {
			for (int i = topIndex; i > -1; i--) {
				std::cout << reinterpret_cast<uintptr_t*>(elements[i]) << " ";
			}
			std::cout << std::endl;
		}
	};

public:
	StackAllocator(size_t totalBytes);
	~StackAllocator();
	
	void* Allocate(size_t neededBytes, size_t alignment);
	virtual void Deallocate(void* pMemory);
	virtual void PrintAddresses();

private:
	Stack m_allocatedObjects;

	void* MakeSpace(size_t bytes);
	void FreeSpace(uint8_t* pMemory);
	virtual void Clear();

	StackAllocator(const StackAllocator&) = delete;
	StackAllocator(StackAllocator&&) = delete;
	StackAllocator operator = (const StackAllocator&) = delete;
	StackAllocator operator = (StackAllocator&&) = delete;
};