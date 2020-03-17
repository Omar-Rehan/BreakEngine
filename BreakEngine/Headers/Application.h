#pragma once
#include "StackAllocator.h"
#include "PoolAllocator.h"

class Application {
public:
	Application() = default;
	void Initialize();
	void Terminate();


private:
	Application(const Application& a) = delete;
	Application(Application&& a) = delete;
	Application operator = (const Application& a) = delete;
	Application operator = (Application&& a) = delete;
};