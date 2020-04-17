#pragma once
#include "GLFW/glfw3.h"
#include "Camera.h"

class Keyboard {
public:
	Keyboard(GLFWwindow* pWindow);

	void Update(float deltaTime);

	GLFWwindow* GetWindowPointer() const;
	
	void AttachCamera(Camera* pCamera);
	void DettachCamera();

private:
	Camera* m_pCamera;
	GLFWwindow* m_pWindow;
	
	void MoveCamera(Camera::Direction direction, float deltaTime);
};