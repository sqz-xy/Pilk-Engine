#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <vector>

class Camera final 
{
public:

	enum Direction
	{
		Forward,
		Backward,
		Left,
		Right
	};

	explicit Camera(const glm::vec3 p_pos, const glm::vec3 p_target, const float p_sens, const int p_vpWidth, const int p_vpHeight);
	~Camera();
	
	void MoveCamera(const Direction p_direction, const float p_distance, const float p_dt);
	void UpdateCamera(const int p_shaderHandle);

	glm::mat4 m_view;
	glm::mat4 m_proj;

	glm::vec3 m_cameraPos;
	const glm::vec3 m_globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_cameraRight;
	glm::vec3 m_cameraDirection;

private:

	glm::vec2 m_lastMousePos;
	float m_pitch;
	float m_yaw;
	float m_sensitivity;
	bool m_mouseMoved;
};