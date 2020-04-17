#pragma once
#include <Timer.h>

class ButtonSequence {
private:
	unsigned int m_crntButton;
	std::vector<int> m_buttons;
	
	Timer m_timer;
	float m_maxTimeAllowed;

	std::function<void()> m_sequenceCompleteCallbackFunction;
	std::function<int()> m_buttonCheckFunction;

	bool IsComplete();
	void Reset();

public:
	ButtonSequence(std::vector<int> buttons, float maxTimeAllowed, std::function<void()> m_sequenceCompleteCallbackFunction, std::function<int()> buttonCheckFunction);

	void Check();
};