#pragma once
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

class Camera {
public:
	static const glm::vec3 s_globalUp;
	enum class Direction {Forward, Backward, Right, Left, Upward, Downward};

	Camera();
	void Zoom(float value);
	void Move(Direction direction, float deltaTime);
	void Rotate(float deltaX, float deltaY);

	glm::vec3 GetPosition() const;
	glm::vec3 GetFront() const;
	glm::mat4 GetViewMatrix() const;
	float GetFieldOfView() const;


private:
	float m_pitch, m_yaw;
	glm::vec3 m_position, m_front;
	float m_movementSpeed, m_fieldOfView;
};
