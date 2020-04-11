#include "pch.h"
#include "GlfwClass.h"

std::vector<GlfwClass*> GlfwClass::s_glfws;


GlfwClass* GlfwClass::GetGLFW(GLFWwindow* pWindow) {
	for (GlfwClass* pGlfw : s_glfws)
		if (pGlfw->GetWindowPointer() == pWindow) return pGlfw;
	return nullptr;
}

void GlfwClass::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void GlfwClass::cursorPositionCallback(GLFWwindow* window, double xPosition, double yPosition) {
	GlfwClass* glfw = GlfwClass::GetGLFW(window);
	Mouse* mouse = glfw->GetMouse();
	Camera* camera = glfw->GetCamera();

	mouse->Update((float)xPosition, (float)yPosition);
	camera->Rotate(mouse->GetDeltaX(), mouse->GetDeltaY());
}
void GlfwClass::scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	GlfwClass* glfw = GlfwClass::GetGLFW(window);
	Camera* camera = glfw->GetCamera();

	camera->Zoom((float)yOffset);
}



void GlfwClass::Initialize() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	m_pWindow = glfwCreateWindow(800, 600, "Window Title", NULL, NULL);
	if (m_pWindow == nullptr) throw - 1;
	glfwMakeContextCurrent(m_pWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		//std::cout << "GLEW Initialization Error: " << glewGetErrorString(glewInit()) << std::endl;
		//return -1;
	}

	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(m_pWindow, GlfwClass::frameBufferSizeCallback);
	glfwSetCursorPosCallback(m_pWindow, GlfwClass::cursorPositionCallback);
	glfwSetScrollCallback(m_pWindow, GlfwClass::scrollCallback);

	s_glfws.push_back(this);
}
void GlfwClass::Terminate() {
	glfwDestroyWindow(m_pWindow);
}

void GlfwClass::SetMouse(Mouse* pMouse) {
	m_pMouse = pMouse;
}
Mouse* GlfwClass::GetMouse() {
	return m_pMouse;
}

void GlfwClass::SetCamera(Camera* pCamera) {
	m_pCamera = pCamera;
}
Camera* GlfwClass::GetCamera() {
	return m_pCamera;
}

GLFWwindow* GlfwClass::GetWindowPointer() const {
	return m_pWindow;
}
bool GlfwClass::WindowShouldClose() {
	return glfwWindowShouldClose(m_pWindow);
}
void GlfwClass::HandleKeyboardInput(float deltaTime) {
	/// Close window
	if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_pWindow, true);

	/// Camera Movement
	GlfwClass* glfw = GlfwClass::GetGLFW(m_pWindow);
	Camera* camera = glfw->GetCamera();

	if (glfwGetKey(m_pWindow, GLFW_KEY_W))
		camera->Move(Camera::Direction::Forward, deltaTime);
	if (glfwGetKey(m_pWindow, GLFW_KEY_S))
		camera->Move(Camera::Direction::Backward, deltaTime);
	if (glfwGetKey(m_pWindow, GLFW_KEY_D))
		camera->Move(Camera::Direction::Right, deltaTime);
	if (glfwGetKey(m_pWindow, GLFW_KEY_A))
		camera->Move(Camera::Direction::Left, deltaTime);
	if (glfwGetKey(m_pWindow, GLFW_KEY_E))
		camera->Move(Camera::Direction::Upward, deltaTime);
	if (glfwGetKey(m_pWindow, GLFW_KEY_Q))
		camera->Move(Camera::Direction::Downward, deltaTime);
}