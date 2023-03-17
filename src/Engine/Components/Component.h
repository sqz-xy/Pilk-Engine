// Abstract component definition
#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
using namespace glm;

class Component 
{
public:

	//virtual ~Component();

private:

};

class ComponentTransform : public Component
{
public:
	explicit ComponentTransform(vec3 p_translation, vec3 p_rotation, vec3 p_scale)
	{
		m_translation = p_translation;
		m_rotation = p_rotation;
		m_scale = p_scale;
		m_transform = mat4(1.0f);
		UpdateTransformation();
	}

	vec3 m_translation;
	vec3 m_rotation;
	vec3 m_scale;
	mat4 m_transform;

	void ComponentTransform::UpdateScale(vec3 p_scale)
	{
		m_scale = p_scale;
		UpdateTransformation();
	}

	void ComponentTransform::UpdateRotation(vec3 p_rotation)
	{
		m_rotation = p_rotation;
		UpdateTransformation();
	}

	void ComponentTransform::UpdateTranslation(vec3 p_translation)
	{
		m_translation = p_translation;
		UpdateTransformation();
	}

private:

	void ComponentTransform::UpdateTransformation()
	{
		mat4 identity(1.0f);

		mat4 scale_mat = scale(identity, m_scale);

		mat4 x_rotate = rotate(scale_mat, m_rotation.x, vec3(1,0,0));
		mat4 y_rotate = rotate(scale_mat, m_rotation.y, vec3(0,1,0));
		mat4 z_rotate = rotate(scale_mat, m_rotation.z, vec3(0,0,1));

		mat4 translation_mat = translate(scale_mat, m_translation);

		m_transform = translation_mat;
	}

};

//struct ComponentTransform 
//{
//	glm::vec3 Position;
//	glm::vec3 Rotation;
//	glm::vec3 Scale;
//
//	glm::mat4 Transform;
//
//	ComponentTransform(glm::vec3 p_pos, glm::vec3 p_rot, glm::vec3 p_scale)
//	{
//		Position = p_pos;
//		Rotation = p_rot;
//		Scale = p_scale;
//
//		UpdateMatrix(p_pos, p_rot, p_scale);
//	}
//
//	ComponentTransform::UpdateMatrix(glm::vec3 p_pos, glm::vec3 p_rot, glm::vec3 p_scale)
//	{
//		glm::mat4 identity(1.0f);
//
//		// Create matrices for pos, rotxyz and scale
//
//		// Transform *= ...
//	}
//};