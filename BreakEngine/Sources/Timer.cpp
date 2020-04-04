#include "pch.h"
#include "Timer.h"

Timer::Timer() {
	m_prevFrameTime = m_crntFrameTime = m_deltaTime = 0.0f;
}

void Timer::Mark() {
	m_crntFrameTime = static_cast<float>(glfwGetTime());
	m_deltaTime = m_crntFrameTime - m_prevFrameTime;
	m_prevFrameTime = m_crntFrameTime;
}
float Timer::GetDeltaTime() const {
	return m_deltaTime;
}
float Timer::GetTimeNow() const {
	return static_cast<float>(glfwGetTime());
}