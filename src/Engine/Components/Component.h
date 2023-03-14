// Abstract component definition
#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

struct ComponentTransform 
{
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	glm::mat4 Transform;

	ComponentTransform(glm::vec3 p_pos, glm::vec3 p_rot, glm::vec3 p_scale)
	{
		Position = p_pos;
		Rotation = p_rot;
		Scale = p_scale;

		UpdateMatrix(p_pos, p_rot, p_scale);
	}

	ComponentTransform::UpdateMatrix(glm::vec3 p_pos, glm::vec3 p_rot, glm::vec3 p_scale)
	{
		glm::mat4 identity(1.0f);

		// Create matrices for pos, rotxyz and scale

		// Transform *= ...
	}
};