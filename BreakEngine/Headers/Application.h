#pragma once
#include "StackAllocator.h"
#include "PoolAllocator.h"
#include "FileManager.h"
#include "ResourceManager.h"

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

public:
	FileManager* m_pFileManager = nullptr;
	ResourceManager* m_pResourceManager = nullptr;
};