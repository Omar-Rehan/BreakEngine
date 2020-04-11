#include "pch.h"
#include "Timer.h"

Timer::Timer() {
	m_timeDeltasSum = m_prevFrameTime = m_crntFrameTime = m_deltaTime = 0.0f;
}

void Timer::Mark() {
	m_crntFrameTime = static_cast<float>(glfwGetTime());
	m_deltaTime = m_crntFrameTime - m_prevFrameTime;
	m_prevFrameTime = m_crntFrameTime;

	m_timeDeltasSum += m_deltaTime;
	m_timeDeltas.push(m_deltaTime);
	if (m_timeDeltas.size() > 100) {
		m_timeDeltasSum -= m_timeDeltas.front();
		m_timeDeltas.pop();
	}
}
float Timer::GetDeltaTime() const {
	return m_deltaTime;
}
float Timer::GetDeltaTimeAverage() const {
	return m_timeDeltasSum / m_timeDeltas.size();
}
float Timer::GetTimeNow() const {
	return static_cast<float>(glfwGetTime());
}