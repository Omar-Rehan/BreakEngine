#include "pch.h"
#include "GLFW.h"

std::vector<GLFW*> GLFW::s_instances;


void GLFW::Initialize() {
	glfwInit();

	/// Initialize OpenGL context
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
	}

	/// Initialize Window
	{
		m_pWindow = glfwCreateWindow(800, 600, "Window Title", NULL, NULL);
		assert(m_pWindow != nullptr);
		glfwMakeContextCurrent(m_pWindow);
	}

	/// Link OpenGL functions to GPU driver functions
	{
		glewExperimental = GL_TRUE;
		assert(glewInit() == GLEW_OK);
	}

	/// Initialize Input
	{
		m_pMouse = new Mouse(m_pWindow);
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetScrollCallback(m_pWindow, Mouse::ScrollCallback);
		glfwSetCursorPosCallback(m_pWindow, Mouse::CursorPositionCallback);
		glfwSetFramebufferSizeCallback(m_pWindow, Mouse::FrameBufferSizeCallback);

		m_pKeyboard = new Keyboard(m_pWindow);

		try { m_pJoystick = new Joystick(m_pWindow); }
		catch (bool error) { if (error) delete m_pJoystick; }
	}

	s_instances.push_back(this);
}
void GLFW::Terminate() {
	if (m_pJoystick != nullptr) delete m_pJoystick;
	if (m_pKeyboard != nullptr) delete m_pKeyboard;
	if (m_pMouse != nullptr) delete m_pMouse;
	glfwDestroyWindow(m_pWindow);
}


void GLFW::SwapBuffers() {
	glfwSwapBuffers(GetWindowPointer());
}
void GLFW::PollEvents(float deltaTime) {
	if (m_pKeyboard != nullptr) m_pKeyboard->Update(deltaTime);
	if (m_pJoystick != nullptr) m_pJoystick->Update(deltaTime);
	glfwPollEvents(); // for HID events (callback functions)
}


void GLFW::AttachCamera(Camera* pCamera) {
	if (m_pMouse != nullptr) m_pMouse->AttachCamera(pCamera);
	if (m_pKeyboard != nullptr) m_pKeyboard->AttachCamera(pCamera);
	if (m_pJoystick != nullptr) m_pJoystick->AttachCamera(pCamera);
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void GLFW::DettachCamera() {
	m_pMouse->DettachCamera();
	m_pKeyboard->DettachCamera();
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
void GLFW::RegisterButtonSequence(HID device, std::vector<int> buttons, float maxTimeAllowed, std::function<void()> sequenceCompleteCallbackFunction) {
	if (device == HID::Joystick && m_pJoystick != nullptr) m_pJoystick->RegisterButtonSequence(buttons, maxTimeAllowed, sequenceCompleteCallbackFunction);
}

bool GLFW::WindowShouldClose() {
	return glfwWindowShouldClose(m_pWindow);
}
GLFWwindow* GLFW::GetWindowPointer() const {
	return m_pWindow;
}


GLFW* GLFW::GetGLFWPointer(GLFWwindow* pWindow) {
	auto iterator = std::find_if(s_instances.begin(), s_instances.end(), [&] (const GLFW* glfw) {return glfw->GetWindowPointer() == pWindow;});
	if (iterator == s_instances.end()) return nullptr;
	return *iterator;
}