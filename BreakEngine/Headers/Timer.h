#pragma once
#include "GLFW/glfw3.h"

class Timer {
public:
	Timer();
	void Mark();
	float GetDeltaTime() const;
	float GetDeltaTimeAverage() const;
	float GetTimeNow() const;

private:
	float m_timeDeltasSum;
	std::queue<float> m_timeDeltas;

	float m_prevFrameTime, m_crntFrameTime, m_deltaTime;
};