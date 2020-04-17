#pragma once
#include "GLFW/glfw3.h"
#include "Camera.h"


class Mouse {
public:
    Mouse(GLFWwindow* pWindow);
    void Update(float nextX, float nextY);
    
    float GetDeltaX();
    float GetDeltaY();
    GLFWwindow* GetWindowPointer() const;

    void AttachCamera(Camera* pCamera);
    void DettachCamera();
    void RotateCamera(float deltaX, float deltaY);
    void ZoomCamera(float yOffset);


    static Mouse* GetInstance(GLFWwindow* pWindow);

    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    static void CursorPositionCallback(GLFWwindow* window, double xPosition, double yPosition);
    static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

private:
    bool m_bFirstFrame;
    float m_sensitivity;
    float m_crntX, m_crntY, m_deltaX, m_deltaY;

    Camera* m_pCamera;
    GLFWwindow* m_pWindow;

    static std::vector<Mouse*> s_instances;
};