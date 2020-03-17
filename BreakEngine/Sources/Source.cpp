#include "Application.h"

void TestStackAllocator() {
	StackAllocator stk(222);
	stk.PrintAddresses();

	void* p1 = stk.Allocate(4, 4);
	stk.PrintAddresses();

	void* p2 = stk.Allocate(16, 8);
	stk.PrintAddresses();

	stk.Deallocate(p2);
	stk.PrintAddresses();

	stk.Deallocate(p1);
	stk.PrintAddresses();
}
void TestPoolAllocator() {
	PoolAllocator p(4, 4, 2);
	p.PrintAddresses();

	void* p1 = p.Allocate();
	p.PrintAddresses();

	void* p2 = p.Allocate();
	p.PrintAddresses();

	p.Deallocate(p1);
	p.PrintAddresses();

	p.Deallocate(p2);
	p.PrintAddresses();
}

int main() {
	TestStackAllocator();
	TestPoolAllocator();


	
	return 0;
}