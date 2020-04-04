#include "pch.h"
#include "Camera.h"

const glm::vec3 Camera::s_globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera::Camera() {
	m_pitch = 0.0f;
	m_yaw = -90.0f;

	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);

	m_movementSpeed = 2.0f;
	m_fieldOfView = 45.0f;
}

void Camera::Zoom(float value) {
	m_fieldOfView = glm::min(glm::max(m_fieldOfView - value, 1.0f), 45.0f);
}
void Camera::Move(Direction direction, float deltaTime) {
	switch (direction) {
	case Direction::Forward:
		m_position += m_front * m_movementSpeed * deltaTime;
		break;
	case Direction::Backward:
		m_position -= m_front * m_movementSpeed * deltaTime;
		break;
	case Direction::Right:
		m_position += glm::cross(m_front, s_globalUp) * m_movementSpeed * deltaTime;
		break; 
	case Direction::Left:
		m_position -= glm::cross(m_front, s_globalUp) * m_movementSpeed * deltaTime;
		break;
	case Direction::Upward:
		m_position += glm::cross(glm::cross(m_front, s_globalUp), m_front) * m_movementSpeed * deltaTime;
		break;
	case Direction::Downward:
		m_position -= glm::cross(glm::cross(m_front, s_globalUp), m_front) * m_movementSpeed * deltaTime;
		break;
	}
}
void Camera::Rotate(float deltaX, float deltaY) {
	/// Update camera angles
	m_yaw += deltaX;
	m_pitch = glm::min(glm::max(m_pitch - deltaY, -89.9f), 89.9f);

	/// Update camera front vector
	m_front.x = glm::cos(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw));
	m_front.y = glm::sin(glm::radians(m_pitch));
	m_front.z = glm::cos(glm::radians(m_pitch)) * glm::sin(glm::radians(m_yaw));
	m_front = glm::normalize(m_front);
}

glm::vec3 Camera::GetPosition() const {
	return m_position;
}
glm::vec3 Camera::GetFront() const {
	return m_front;
}
glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(m_position, m_position + m_front, s_globalUp);
	
	/// Manual implementation
	{
		glm::vec3 rightAxis = glm::cross(m_front, s_globalUp);
		glm::vec3 upAxis = glm::cross(rightAxis, m_front);

		/// Remember: matrices in glm are column major ones!

		glm::mat4 rotationMatrix = glm::mat4(
			glm::vec4(rightAxis, 0.0f),
			glm::vec4(upAxis, 0.0f),
			glm::vec4(-m_front, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);

		glm::mat4 translationMatrix = glm::mat4(
			1.0f,          0.0f,          0.0f,          0.0f,
			0.0f,          1.0f,          0.0f,			 0.0f,
			0.0f,          0.0f,          1.0f,          0.0f,
			m_position.x,  m_position.y,  m_position.z,  1.0f
		);

		return glm::inverse(translationMatrix * rotationMatrix);
	}
}
float Camera::GetFieldOfView() const {
	return glm::radians(m_fieldOfView);
}