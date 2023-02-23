// Abstract component definition
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <vector>

#pragma once

struct ComponentPosition
{ 
	glm::vec3 position;
};

struct ComponentShader
{
	unsigned int shaderID;
	//might need to also store shader.
};