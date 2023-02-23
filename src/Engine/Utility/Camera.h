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

	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraDirection;
	glm::vec3 m_cameraRight;
	glm::vec3 m_cameraUp;

	glm::mat4 m_view;
	glm::mat4 m_projection;

	explicit Camera(const glm::vec3 p_pos, const glm::vec3 p_target, const glm::vec3 p_direction, const glm::vec3 p_cameraUp, const float p_width, const float p_height);
	~Camera();

	void MoveCamera(const Direction p_direction, const float p_distance, const float p_dt);
	void UpdateCamera(const int p_shaderHandle);
};