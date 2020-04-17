#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Mouse.h"
#include "Keyboard.h"
#include "Joystick.h"

#include "Camera.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3dll.lib")

#include <vector>

class GLFW {
public:
	enum class HID { Keyboard, Mouse, Joystick };

private:
	GLFWwindow* m_pWindow;
	Camera* m_pCamera;
	
	Mouse* m_pMouse;
	Keyboard* m_pKeyboard;
	Joystick* m_pJoystick;

	static std::vector<GLFW*> s_instances;
	
public:
	void Initialize();
	void Terminate();
	
	void SwapBuffers();
	void PollEvents(float deltaTime);

	void AttachCamera(Camera* pCamera);
	void DettachCamera();
	void RegisterButtonSequence(HID device, std::vector<int> buttons, float maxTimeAllowed, std::function<void()> sequenceCompleteCallbackFunction);
		
	bool WindowShouldClose();
	GLFWwindow* GetWindowPointer() const;


	static GLFW* GetGLFWPointer(GLFWwindow* pWindow);
};