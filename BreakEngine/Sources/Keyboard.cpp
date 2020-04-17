#include "pch.h"
#include "Keyboard.h"

Keyboard::Keyboard(GLFWwindow* pWindow) : m_pWindow(pWindow), m_pCamera(nullptr) {}

void Keyboard::Update(float deltaTime) {
	if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_pWindow, true);


	if (glfwGetKey(m_pWindow, GLFW_KEY_W))
		MoveCamera(Camera::Direction::Forward, deltaTime);
	if (glfwGetKey(m_pWindow, GLFW_KEY_S))
		MoveCamera(Camera::Direction::Backward, deltaTime);
	if (glfwGetKey(m_pWindow, GLFW_KEY_D))
		MoveCamera(Camera::Direction::Right, deltaTime);
	if (glfwGetKey(m_pWindow, GLFW_KEY_A))
		MoveCamera(Camera::Direction::Left, deltaTime);
	if (glfwGetKey(m_pWindow, GLFW_KEY_E))
		MoveCamera(Camera::Direction::Upward, deltaTime);
	if (glfwGetKey(m_pWindow, GLFW_KEY_Q))
		MoveCamera(Camera::Direction::Downward, deltaTime);
}

GLFWwindow* Keyboard::GetWindowPointer() const {
	return m_pWindow;
}

void Keyboard::AttachCamera(Camera* pCamera) {
	m_pCamera = pCamera;
}
void Keyboard::DettachCamera() {
	m_pCamera = nullptr;
}
void Keyboard::MoveCamera(Camera::Direction direction, float deltaTime) {
	if (m_pCamera != nullptr) m_pCamera->Move(direction, deltaTime);
}