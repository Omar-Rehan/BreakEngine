#include "pch.h"
#include "Mouse.h"

std::vector<Mouse*> Mouse::s_instances;
Mouse* Mouse::GetInstance(GLFWwindow* pWindow) {
    auto iterator = std::find_if(s_instances.begin(), s_instances.end(), [&](const Mouse* pMouse) {return pMouse->GetWindowPointer() == pWindow; });
    if (iterator == s_instances.end()) return nullptr;
    return *iterator;
}


Mouse::Mouse(GLFWwindow* pWindow) : m_pWindow(pWindow), m_pCamera(nullptr) { 
    m_bFirstFrame = true; 
    m_sensitivity = 0.8f; 
    
    m_crntX = m_crntY = 0.0f;
    m_deltaX = m_deltaY = 0.0f;

    s_instances.push_back(this);
}
void Mouse::Update(float nextX, float nextY) {
    if (m_bFirstFrame) {
        m_crntX = nextX;
        m_crntY = nextY;
        m_bFirstFrame = false;
    }

    m_deltaX = (nextX - m_crntX) * m_sensitivity;
    m_deltaY = (nextY - m_crntY) * m_sensitivity;

    m_crntX = nextX;
    m_crntY = nextY;
}

float Mouse::GetDeltaX() { return m_deltaX; }
float Mouse::GetDeltaY() { return m_deltaY; }
GLFWwindow* Mouse::GetWindowPointer() const {
    return m_pWindow;
}

void Mouse::AttachCamera(Camera* pCamera) {
    m_pCamera = pCamera;
}
void Mouse::DettachCamera() {
    m_pCamera = nullptr;
}
void Mouse::RotateCamera(float deltaX, float deltaY) {
    if (m_pCamera != nullptr) m_pCamera->Rotate(deltaX, deltaY);
}
void Mouse::ZoomCamera(float yOffset) {
    if (m_pCamera != nullptr) m_pCamera->Zoom(yOffset);
}


/// Callback Functions
void Mouse::FrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void Mouse::CursorPositionCallback(GLFWwindow* window, double xPosition, double yPosition) {
    Mouse* pMouse = Mouse::GetInstance(window);
    pMouse->Update((float)xPosition, (float)yPosition);
    pMouse->RotateCamera(pMouse->GetDeltaX(), pMouse->GetDeltaY());
}
void Mouse::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    Mouse* pMouse = Mouse::GetInstance(window);
    pMouse->ZoomCamera(yOffset);
}