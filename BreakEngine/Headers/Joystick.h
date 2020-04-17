#pragma once
#include "GLFW/glfw3.h"

#include "Camera.h"
#include "ButtonSequence.h"

class Joystick {
public:
	enum class XBOX_BUTTONS { A, B, X, Y, LB, RB, BACK, START, LSB, RSB, UP, RIGHT, DOWN, LEFT };
	enum class XBOX_AXES { LSX, LSY, RSX, RSY, LT, RT };

private:
	int m_id;
	Camera* m_pCamera;
	static int s_numOfJoysticks;
	static constexpr float s_deadzone = 0.05f;
	
	int m_numOfAxes;
	const float* m_liveAxes = nullptr;
	std::vector<float> m_prevAxes, m_crntAxes;

	int m_numOfButtons;
	const unsigned char* m_liveButtons = nullptr;
	int m_prevButtons, m_crntButtons; // access the bits

	std::vector<ButtonSequence> m_registeredButtonSequences;

	bool ProcessButton(int button);
	float ProcessAxis(int axis);

public:
	Joystick(GLFWwindow* window);
	~Joystick();

	void Update(float deltaTime);

	void AttachCamera(Camera* pCamera);
	void DettachCamera();
	void RotateCamera(float deltaX, float deltaY);
	void MoveCamera(Camera::Direction direction, float deltaTime);

	int DetectButtonsJustWentDown();
	int DetectButtonsJustWentUp();

	void RegisterButtonSequence(std::vector<int> buttons, float maxTimeAllowed, std::function<void()> sequenceCompleteCallbackFunction);
	
	bool ReadButton(int button);
	float ReadAxis(int axis);
};