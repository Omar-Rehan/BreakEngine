#include "pch.h"
#include "ButtonSequence.h"

ButtonSequence::ButtonSequence(std::vector<int> buttons, float maxTimeAllowed, std::function<void()> sequenceCompleteCallbackFunction, std::function<int()> buttonCheckFunction) :
	m_crntButton(0),
	m_buttons(buttons),
	m_maxTimeAllowed(maxTimeAllowed),
	m_sequenceCompleteCallbackFunction(sequenceCompleteCallbackFunction),
	m_buttonCheckFunction(buttonCheckFunction) {

	assert(m_buttons.size() > 1);
}

bool ButtonSequence::IsComplete() {
	return m_crntButton == m_buttons.size();
}
void ButtonSequence::Reset() {
	m_crntButton = 0;
}

void ButtonSequence::Check() {
	assert(m_crntButton < m_buttons.size());

	int buttonMask = (1 << m_crntButton);
	int buttonsDown = m_buttonCheckFunction();

	if (buttonsDown) {
		int x = 4;
	}

	if (buttonsDown & (~buttonMask)) { // another button in the sequence has just been pressed
		Reset();
	
	} else if (buttonsDown & buttonMask) {
		if (m_crntButton == 0) { // first button in the sequence
			m_timer.Mark();
			m_crntButton++;
		
		} else {
			float time = m_timer.GetTimeSinceLastMark();
			if (time < m_maxTimeAllowed) {
				m_crntButton++;
				
				if (IsComplete()) {
					m_sequenceCompleteCallbackFunction();
					Reset();
				}
			} else { // time is up!
				Reset();
			}
		}
	}
}