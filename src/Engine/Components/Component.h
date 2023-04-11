// Abstract component definition
#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "../PilkEngineCommon.h"
#include "../Managers/ResourceManager.h"

using namespace glm;

class Component 
{
public:
	virtual ~Component()
	{

	}

private:

};

// Component By Matthew Liney
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

		mat4 translation_mat = translate(identity, m_translation);

		mat4 scale_mat = scale(translation_mat, m_scale);

		mat4 x_rotate = rotate(scale_mat, m_rotation.x, vec3(1,0,0));
		mat4 y_rotate = rotate(x_rotate, m_rotation.y, vec3(0,1,0));
		mat4 z_rotate = rotate(y_rotate, m_rotation.z, vec3(0,0,1));

		m_transform = z_rotate;
	}

};


//ComponentVelocity created by Eryk
class ComponentVelocity : public Component
{
public:
	ComponentVelocity(const float p_x, const float p_y, const float p_z)
	{
		m_velocity = vec3(p_x, p_y, p_z);
	}

	ComponentVelocity(vec3 p_velocity)
	{
		m_velocity = p_velocity;
	}

	vec3 ComponentVelocity::GetVelocity()
	{
		return m_velocity;
	}

	void ComponentVelocity::SetVelocity(vec3 p_velocity)
	{
		m_velocity = p_velocity;
	}

private:
	vec3 m_velocity;
};

//ComponentCollisionSphere created by Eryk
class ComponentCollisionSphere : public Component
{
public:
	ComponentCollisionSphere(float p_radius)
	{
		m_radius = p_radius;
	}

	float ComponentCollisionSphere::GetCollisionSphere()
	{
		return m_radius;
	}

	void ComponentCollisionSphere::SetCollisionSphere(float p_radius)
	{
		m_radius = p_radius;
	}

private:
	float m_radius;
};

class ComponentGeometry : public Component
{
public:
	ComponentGeometry(char* p_path)
	{
		m_model = ResourceManager::LoadModel(p_path);
	}

	~ComponentGeometry()
	{
		delete m_model;
		m_model = nullptr;
	}

	void Draw(const int p_shaderHandle)
	{
		m_model->Draw(p_shaderHandle);
	}

private:
	Model* m_model;
};


// Component shader created by James
class ComponentShader : public Component
{
public:
	ComponentShader(unsigned int p_shaderProgramID , Model* pModel)
	{
		//m_uniform_modelMat = glGetUniformLocation(p_shaderProgramID, "model");
		m_colour = glGetUniformLocation(p_shaderProgramID, "uColour");
		m_model = pModel;
		m_shaderProgramID = p_shaderProgramID;

		// m_tex = glGetUniformLocation(p_shaderProgramID, "uTextureDiffuse1");
		// m_uniform_projMat = glGetUniformLocation(p_shaderProgramID, "projection");
		// m_uniform_viewMat = glGetUniformLocation(p_shaderProgramID, "view");
	}

	void Render(glm::mat4* pModelMat)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, "uModel"), 1, GL_FALSE, &(*pModelMat)[0][0]);
		m_model->Draw(m_shaderProgramID);

		//glUniform4f(glGetUniformLocation(m_shaderProgramID, "uColour"), m_colour[0], m_colour[1], m_colour[2], m_colour[3]);
		//m_Camera->UpdateCamera(m_shaderProgramID);
		//glUniformMatrix4fv(glGetUniformLocation(*m_shaderProgramID, "uModel"), 1, GL_FALSE, &temp[0][0]);
	}

private:
	int m_uniform_modelMat;
	int m_colour;
	Model* m_model;
	unsigned int m_shaderProgramID;

	//int m_uniform_viewMat;
	//int m_uniform_projMat;
	//int m_tex;
};