#include "Camera.h"

#include "PilkEngineCommon.h"

Camera::Camera(const glm::vec3 p_pos, const glm::vec3 p_target, const float p_width, const float p_height)
{
	m_cameraPos = p_pos;
	m_cameraTarget = p_target;
	m_cameraDirection = glm::vec3(glm::normalize(p_target - p_pos));

	m_yaw = -90.0f;
	m_pitch = 0.0f;
	m_sens = 0.1f;
	m_zoom = 45.0f;
	m_lastMousePos = glm::vec2(0.0f, 0.0f);

	m_projection = glm::perspective(glm::radians(m_zoom), (float)p_width / (float)p_height, 0.1f, 100.0f);

	m_cameraRight = glm::normalize(glm::cross(m_cameraUp, m_cameraDirection));
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

	std::cout << "CamPos: X:" << m_cameraPos.x << " Y: " << m_cameraPos.y << " Z: " << m_cameraPos.z << '\n';
}

void Camera::RotateCamera(glm::vec2& p_mousePos)
{
	float mouseDeltaX = p_mousePos.x - m_lastMousePos.x;
	float mouseDeltaY = p_mousePos.y - m_lastMousePos.y;


	m_lastMousePos = p_mousePos;

	m_yaw += mouseDeltaX * m_sens;
	m_pitch -= mouseDeltaY * m_sens;

	if (m_pitch > 90.0f)
		m_pitch = 90.0f;
	else if (m_pitch < -90.0f)
		m_pitch = -90.0f;
	else
		m_pitch -= mouseDeltaY * m_sens;
	

	m_cameraDirection.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	m_cameraDirection.y = sin(glm::radians(m_pitch));
	m_cameraDirection.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));

	m_cameraRight = glm::normalize(glm::cross(m_cameraUp, m_cameraDirection));
	m_cameraDirection = glm::normalize(m_cameraDirection);
}

void Camera::UpdateCamera()
{
	m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraDirection, m_cameraUp);
}
