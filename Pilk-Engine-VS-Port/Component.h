// Abstract component definition
#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "PilkEngineCommon.h"
#include "ResourceManager.h"
#include "Camera.h"

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

	void UpdateScale(vec3 p_scale)
	{
		m_scale = p_scale;
		UpdateTransformation();
	}

	void UpdateRotation(vec3 p_rotation)
	{
		m_rotation = p_rotation;
		UpdateTransformation();
	}

	void UpdateTranslation(vec3 p_translation)
	{
		m_translation = p_translation;
		UpdateTransformation();
	}

private:

	void UpdateTransformation()
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

	vec3 GetVelocity()
	{
		return m_velocity;
	}

	// set x and y instead of this (takes in float)

	void SetVelVector(vec3 p_velocity)
	{
		m_velocity = p_velocity;
	}

	void SetVelX(float p_x)
	{
		m_velocity.x = p_x;
	}

	void SetVelY(float p_x)
	{
		m_velocity.y = p_x;
	}

	void SetVelZ(float p_x)
	{
		m_velocity.z = p_x;
	}

	vec3 GetGravity()
	{
		return m_gravity;
	}

	void SetGravity(vec3 p_gravity)
	{
		m_gravity = p_gravity;
	}

	vec3 GetCurrentGravity()
	{
		return m_currentGravity;
	}

	void SetCurrentGravity(vec3 p_gravity)
	{
		m_currentGravity = p_gravity;
	}

	// gravity should deduct from velocity

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

	float GetCollisionSphere()
	{
		return m_radius;
	}

	void SetCollisionSphere(float p_radius)
	{
		m_radius = p_radius;
	}

private:
	float m_radius;
};

// component by matthew liney
class ComponentCollisionAABB : public Component
{
public:

	ComponentCollisionAABB(float p_height, float p_width, float p_depth)
	{
		m_height = p_height;
		m_width = p_width;
		m_depth = p_depth;

		m_is_active = true;
	}

	float GetHeight()
	{
		return m_height;
	}

	float GetWidth()
	{
		return m_width;
	}

	float GetDepth()
	{
		return m_depth;
	}

	// add setters later if ur a lamo

private:
	float m_height;
	float m_width;
	float m_depth;

	bool m_is_active;
};

// component by matthew liney
class ComponentCollisionPoint : public Component
{
public:

	// the collision point represents a point relative to the entity, not the point on screen.
	ComponentCollisionPoint(vec3 p_point)
	{

		m_point = p_point;
		m_is_active = true;
	}

	// add setters later if ur a lamo

	vec3 GetPoint()
	{
		return m_point;
	}

	float GetX()
	{
		return m_point.x;
	}

	float GetY()
	{
		return m_point.y;
	}

private:

	vec3 m_point;
	bool m_is_active;
};

// component by matt
// Edited by Eryk
// add whatever you think something in the scene would need
// like health or whatever.
// needed to make this for jumping. players and enemies can both use this.
class ComponentProperties : public Component
{
public:
	ComponentProperties(bool p_hasJumped, float p_health, float p_damage, vec3 p_dir)
	{
		m_hasJumped = p_hasJumped;
		m_health = p_health;
		m_damage = p_damage;
		m_direction = p_dir;
	}

	bool m_hasJumped;
	float m_health;
	float m_damage;
	vec3 m_direction;

private:
};

class ComponentGeometry : public Component
{
public:
	ComponentGeometry(const char* p_path)
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
		if (!ResourceManager::CreateShaderProgram(&m_shaderProgramID, p_vertexPath.c_str(), p_fragmentPath.c_str())) return;
		//m_uniform_modelMat = glGetUniformLocation(p_shaderProgramID, "model");
		m_colour = glGetUniformLocation(m_shaderProgramID, "uColour");
		m_shaderProgramID = m_shaderProgramID;

		// m_tex = glGetUniformLocation(p_shaderProgramID, "uTextureDiffuse1");
		// m_uniform_projMat = glGetUniformLocation(p_shaderProgramID, "projection");
		// m_uniform_viewMat = glGetUniformLocation(p_shaderProgramID, "view");
	}

	void UseShader(glm::mat4* p_modelMat, glm::mat4* p_viewMat, glm::mat4* p_projMat, ComponentTransform* p_trans)
	{
		glUseProgram(m_shaderProgramID);
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, "uModel"), 1, GL_FALSE, &(*p_modelMat)[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, "uView"), 1, GL_FALSE, &(*p_viewMat)[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, "uProj"), 1, GL_FALSE, &(*p_projMat)[0][0]);

		glm::vec2 scale = p_trans->m_scale;
		// TODO: Pass in m_scale
		glUniform2f(glGetUniformLocation(m_shaderProgramID, "uScale"), 1.0, 1.0);

		glUniform1f(glGetUniformLocation(m_shaderProgramID, "uTime"), glfwGetTime());


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