#pragma once
#include "GLFW/glfw3.h"

class Timer {
public:
	Timer();
	void Mark();
	float GetDeltaTime() const;
	float GetTimeNow() const;

private:
	float m_prevFrameTime, m_crntFrameTime, m_deltaTime;
};