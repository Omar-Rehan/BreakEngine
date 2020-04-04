#include "pch.h"
#include "Application.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Camera.h"
#include "Mouse.h"
#include "Timer.h"

#include "GLFW/glfw3.h"
#include "GL/glew.h"


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3dll.lib")



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

GLFWwindow* g_pWindow;
Camera g_camera;
Mouse g_mouse;

int Init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	g_pWindow = glfwCreateWindow(800, 600, "Window Title", NULL, NULL);
	if (g_pWindow == nullptr) throw - 1;
	glfwMakeContextCurrent(g_pWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW Initialization Error: " << glewGetErrorString(glewInit()) << std::endl;
		return -1;
	}
	

}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void cursorPositionCallback(GLFWwindow* window, double xPosition, double yPosition) {
	g_mouse.Update((float)xPosition, (float)yPosition);
	g_camera.Rotate(g_mouse.GetDeltaX(), g_mouse.GetDeltaY());
}
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	g_camera.Zoom((float)yOffset);
}
void HandleKeyboardInput(GLFWwindow* window, float deltaTime) {
	/// Close window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	/// Camera Movement
	if (glfwGetKey(window, GLFW_KEY_W))
		g_camera.Move(Camera::Direction::Forward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S))
		g_camera.Move(Camera::Direction::Backward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D))
		g_camera.Move(Camera::Direction::Right, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A))
		g_camera.Move(Camera::Direction::Left, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E))
		g_camera.Move(Camera::Direction::Upward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q))
		g_camera.Move(Camera::Direction::Downward, deltaTime);
}

int main() {
	{
		Init();
		glViewport(0, 0, 800, 600);
		glfwSetInputMode(g_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetFramebufferSizeCallback(g_pWindow, frameBufferSizeCallback);
		glfwSetCursorPosCallback(g_pWindow, cursorPositionCallback);
		glfwSetScrollCallback(g_pWindow, scrollCallback);
		
		/// Color stuff
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		/// Depth stuff
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}


	Application app;
	app.Initialize();
	app.m_pFileManager->Mount("Assets/Nanosuit Model.zip");
	Model m = app.m_pResourceManager->GetModel("Nanosuit Model/nanosuit.obj");

	ShaderProgram s = ShaderProgram("Shaders/SimpleNanosuitVShader.glsl", "Shaders/SimpleNanosuitFShader.glsl");
	
	Timer timer;
	srand(timer.GetTimeNow());
	while (!glfwWindowShouldClose(g_pWindow)) {
		timer.Mark();
		HandleKeyboardInput(g_pWindow, timer.GetDeltaTime());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, -5.0f));
		glm::mat4 viewMat = g_camera.GetViewMatrix();
		glm::mat4 projMat = glm::perspective(g_camera.GetFieldOfView(), 4.0f / 3.0f, 0.1f, 100.0f);
		s.SetFloatMatrix4Uniform("modelMat", modelMat);
		s.SetFloatMatrix4Uniform("viewMat", viewMat);
		s.SetFloatMatrix4Uniform("projMat", projMat);
		m.Render(s);

		glfwPollEvents();
		glfwSwapBuffers(g_pWindow);
	}

	
	return 0;
}