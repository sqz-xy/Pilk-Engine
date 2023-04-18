// Abstract component definition
#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "../PilkEngineCommon.h"
#include "../Managers/ResourceManager.h"
#include "../Utility/Camera.h"

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
//Edited by: Matthew Liney
//it's called component physics now
class ComponentPhysics : public Component
{
public:
	//ComponentPhysics(const float p_x, const float p_y, const float p_z)
	//{
	//	m_velocity = vec3(p_x, p_y, p_z);
	//} dumb

	ComponentPhysics(vec3 p_velocity, vec3 p_gravity)
	{
		m_velocity = p_velocity;
		m_gravity = p_gravity;
		m_currentGravity = vec3(0.0f, 0.0f, 0.0f);
	}

	vec3 ComponentPhysics::GetVelocity()
	{
		return m_velocity;
	}

	void ComponentPhysics::SetVelocity(vec3 p_velocity)
	{
		m_velocity = p_velocity;
	}

	vec3 ComponentPhysics::GetGravity()
	{
		return m_gravity;
	}

	void ComponentPhysics::SetGravity(vec3 p_gravity)
	{
		m_gravity = p_gravity;
	}

	vec3 ComponentPhysics::GetCurrentGravity()
	{
		return m_currentGravity;
	}

	void ComponentPhysics::SetCurrentGravity(vec3 p_gravity)
	{
		m_currentGravity = p_gravity;
	}

private:
	vec3 m_velocity;
	vec3 m_gravity; // value to be added onto current value
	vec3 m_currentGravity; // current value, added (or subtracted) to the position
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
	unsigned int m_shaderProgramID;

	ComponentShader(const std::string& p_vertexPath, const std::string& p_fragmentPath)
	{
		if (!ResourceManager::CreateShaderProgram(&m_shaderProgramID, "resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag")) return;
		//m_uniform_modelMat = glGetUniformLocation(p_shaderProgramID, "model");
		m_colour = glGetUniformLocation(m_shaderProgramID, "uColour");
		m_shaderProgramID = m_shaderProgramID;

		// m_tex = glGetUniformLocation(p_shaderProgramID, "uTextureDiffuse1");
		// m_uniform_projMat = glGetUniformLocation(p_shaderProgramID, "projection");
		// m_uniform_viewMat = glGetUniformLocation(p_shaderProgramID, "view");
	}

	void UseShader(glm::mat4* p_modelMat, glm::mat4* p_viewMat, glm::mat4* p_projMat)
	{
		glUseProgram(m_shaderProgramID);
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, "uModel"), 1, GL_FALSE, &(*p_modelMat)[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, "uView"), 1, GL_FALSE, &(*p_viewMat)[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, "uProj"), 1, GL_FALSE, &(*p_projMat)[0][0]);

		//glUniform4f(glGetUniformLocation(m_shaderProgramID, "uColour"), m_colour[0], m_colour[1], m_colour[2], m_colour[3]);
		//m_Camera->UpdateCamera(m_shaderProgramID);
		//glUniformMatrix4fv(glGetUniformLocation(*m_shaderProgramID, "uModel"), 1, GL_FALSE, &temp[0][0]);
	}

private:
	int m_uniform_modelMat;
	int m_colour;

	//int m_uniform_viewMat;
	//int m_uniform_projMat;
	//int m_tex;
};