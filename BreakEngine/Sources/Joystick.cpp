#include "pch.h"
#include "Joystick.h"

int Joystick::s_numOfJoysticks = 0;

Joystick::Joystick(GLFWwindow* window) {
	m_id = s_numOfJoysticks;
	if (!glfwJoystickPresent(m_id)) {
		std::cout << "Cannot read new joystick" << std::endl;
		throw true;
	}
	s_numOfJoysticks++;

	m_pCamera = nullptr;

	m_liveButtons = glfwGetJoystickButtons(m_id, &m_numOfButtons);
	m_liveAxes = glfwGetJoystickAxes(m_id, &m_numOfAxes);

	m_crntButtons = m_prevButtons = 0;
	m_prevAxes = std::vector<float>(m_numOfAxes, 0.0f);
}
Joystick::~Joystick() {
	s_numOfJoysticks--;
}

void Joystick::Update(float deltaTime) {
	/// Read GLFW's buttons & axes arrays
	{
		m_liveButtons = glfwGetJoystickButtons(m_id, &m_numOfButtons);
		m_liveAxes = glfwGetJoystickAxes(m_id, &m_numOfAxes);
	}

	/// Calculate this frame's buttons & axes states (process GLFW's arrays)
	{
		m_crntButtons = 0;
		for (int i = m_numOfButtons; i >= 0; i--)
			m_crntButtons = (m_crntButtons << 1) + ProcessButton(i);

		for (int i = 0; i < m_numOfAxes; i++)
			m_crntAxes.push_back(ProcessAxis(i));
	}

	/// Check registered button sequences
	{
		for (auto& sequence : m_registeredButtonSequences) sequence.Check();
	}

	/// Check camera updates
	if (m_pCamera != nullptr) {
		/// Camera rotation
		RotateCamera(ReadAxis((int)XBOX_AXES::RSX), ReadAxis((int)XBOX_AXES::RSY));
		
		/// Camera movement
		if (ReadButton((int)XBOX_BUTTONS::UP))    MoveCamera(Camera::Direction::Forward, deltaTime);
		if (ReadButton((int)XBOX_BUTTONS::DOWN))  MoveCamera(Camera::Direction::Backward, deltaTime);
		if (ReadButton((int)XBOX_BUTTONS::RIGHT)) MoveCamera(Camera::Direction::Right, deltaTime);
		if (ReadButton((int)XBOX_BUTTONS::LEFT))  MoveCamera(Camera::Direction::Left, deltaTime);
		if (ReadButton((int)XBOX_BUTTONS::RB))  MoveCamera(Camera::Direction::Upward, deltaTime);
		if (ReadButton((int)XBOX_BUTTONS::LB))  MoveCamera(Camera::Direction::Downward, deltaTime);

		MoveCamera(Camera::Direction::Forward, deltaTime * ReadAxis((int)XBOX_AXES::LSY));
		MoveCamera(Camera::Direction::Right, deltaTime * ReadAxis((int)XBOX_AXES::LSX));
	}

	/// Save the states for next frame
	{
		m_prevButtons = m_crntButtons;
		m_prevAxes = std::move(m_crntAxes);
	}
}

void Joystick::AttachCamera(Camera* pCamera) {
	m_pCamera = pCamera;
}
void Joystick::DettachCamera() {
	m_pCamera = nullptr;
}
void Joystick::RotateCamera(float deltaX, float deltaY) {
	if (m_pCamera != nullptr) m_pCamera->Rotate(deltaX, -deltaY);
}
void Joystick::MoveCamera(Camera::Direction direction, float deltaTime) {
	if (m_pCamera != nullptr) m_pCamera->Move(direction, deltaTime);
}

bool Joystick::ProcessButton(int button) {
	return m_liveButtons[button] == GLFW_PRESS;
}
float Joystick::ProcessAxis(int axis) {	
	float value = m_liveAxes[axis];
	if (axis == (int)XBOX_AXES::LSY || axis == (int)XBOX_AXES::RSY) value = -value;

	if (axis == (int)XBOX_AXES::LT || axis == (int)XBOX_AXES::RT)
		return (std::abs(value + 1) > s_deadzone ? value : -1.0f);
	return (std::abs(value) > s_deadzone ? value : 0.0f);
}

bool Joystick::ReadButton(int button) {
	return (m_crntButtons & (1 << button));
}
float Joystick::ReadAxis(int axis) {
	return m_crntAxes[axis];
}


int Joystick::DetectButtonsJustWentDown() {
	int buttonsChanged = m_crntButtons ^ m_prevButtons;
	return buttonsChanged & m_crntButtons;
}
int Joystick::DetectButtonsJustWentUp() {
	int buttonsChanged = m_crntButtons ^ m_prevButtons;
	return buttonsChanged & (~m_crntButtons);
}


void Joystick::RegisterButtonSequence(std::vector<int> buttons, float maxTimeAllowed, std::function<void()> sequenceCompleteCallbackFunction) {
	std::function<int()> buttonCheckFunction = std::bind(&Joystick::DetectButtonsJustWentDown, this);
	//std::function<int()> buttonCheckFunction = [&]() { return this->DetectButtonsJustWentDown(); };
	ButtonSequence buttonSequence = ButtonSequence(buttons, maxTimeAllowed, sequenceCompleteCallbackFunction, buttonCheckFunction);
	m_registeredButtonSequences.push_back(buttonSequence);
}