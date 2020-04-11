#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Camera.h"
#include "Mouse.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3dll.lib")

#include <vector>

class GlfwClass {
private:
	GLFWwindow* m_pWindow;
	Camera* m_pCamera;
	Mouse* m_pMouse;

	static std::vector<GlfwClass*> s_glfws;

public:
	void Initialize();
	void Terminate();
	
	void SetMouse(Mouse* pMouse);
	Mouse* GetMouse();
	
	void SetCamera(Camera* pCamera);
	Camera* GetCamera();
	
	GLFWwindow* GetWindowPointer() const;
	bool WindowShouldClose();
	void HandleKeyboardInput(float deltaTime);

	
	static GlfwClass* GetGLFW(GLFWwindow* pWindow);
	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void cursorPositionCallback(GLFWwindow* window, double xPosition, double yPosition);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};