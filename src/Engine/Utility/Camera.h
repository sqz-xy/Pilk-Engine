#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <vector>

class Camera final 
{
public:

	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraDirection;
};