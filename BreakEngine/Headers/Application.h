#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLFW.h"
#include "FileManager.h"
#include "ResourceManager.h"
#include "JobSystem.h"
#include "StackAllocator.h"
#include "PoolAllocator.h"
#include "Timer.h"


class Application {
public:
	Application() = default;
	void Initialize();
	void Terminate();
	void GameLoop();


private:
	Application(const Application& a) = delete;
	Application(Application&& a) = delete;
	Application operator = (const Application& a) = delete;
	Application operator = (Application&& a) = delete;


	void TestStackAllocator();
	void TestPoolAllocator();
	
	void TestJobSystem(JobSystem* jobSystem);
	static void GetOddsSum(uintptr_t parameters);

	void TestJobSystem2(JobSystem* jobSystem, Model& m, ShaderProgram& s);
	static void CalculateTransformationMatrices(uintptr_t matrices);

	void TestButtonSequence();
	void PrintSequenceDetected();

public:
	Camera m_camera;
	GLFW* m_pGLFW = nullptr;
	FileManager* m_pFileManager = nullptr;
	ResourceManager* m_pResourceManager = nullptr;
};