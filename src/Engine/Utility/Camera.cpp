#include "Camera.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <glad.h>

Camera::Camera(const glm::vec3 p_pos, const glm::vec3 p_target, const float p_sens, const int p_vpWidth, const int p_vpHeight) : 
																																m_cameraPos(p_pos),
																																m_cameraDirection(glm::normalize(p_target - p_pos)), 
																																m_cameraRight(glm::normalize(glm::cross(m_globalUp, m_cameraDirection))),
																																m_view(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -2.0f))), 
																																m_proj(glm::perspective(1.0f, static_cast<float>(p_vpWidth / p_vpHeight), 0.5f, 100.0f)), 
																																m_sensitivity(p_sens), m_mouseMoved(false), 
																																m_lastMousePos(glm::vec2(0, 0)), m_pitch(0), m_yaw(0)
{
	
}

Camera::~Camera()
{

}

void Camera::MoveCamera(const Direction p_direction, const float p_distance, const float p_dt)
{
	const float camSpeed = p_distance * p_dt;

	switch (p_direction)
	{
	case Forward:
		m_cameraPos += m_cameraDirection * camSpeed;
		break;
	case Backward:
		m_cameraPos -= m_cameraDirection * camSpeed;
		break;
	case Left:
		m_cameraPos += m_cameraRight * camSpeed;
		break;
	case Right:
		m_cameraPos -= m_cameraRight * camSpeed;
		break;
	default:
		break;
	}
}

void Camera::UpdateCamera(const int p_shaderHandle)
{
	m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraDirection, m_globalUp);

	glUseProgram(p_shaderHandle);
	glUniformMatrix4fv(glGetUniformLocation(p_shaderHandle, "uView"), 1, GL_FALSE, &m_view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(p_shaderHandle, "uProj"), 1, GL_FALSE, &m_proj[0][0]);
}
