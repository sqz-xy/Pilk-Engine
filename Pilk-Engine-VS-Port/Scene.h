#pragma once

#include "SceneManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

// By Thomas Beet
class Scene
{
	friend class SceneManager;
public:
	explicit Scene(SceneManager *p_sceneManager);
	virtual ~Scene();

	virtual void Close();

	/// <summary>
	/// Render logic
	/// </summary>
	/// <param name="p_dt">delta time</param>
	virtual void Render(float p_dt) const = 0;

	/// <summary>
	/// Update logic
	/// </summary>
	/// <param name="p_dt">delta time</param>
	virtual void Update(float p_dt) = 0;

	/// <summary>
	/// Load logic
	/// </summary>
	virtual void Load() = 0;

	/// <summary>
	/// Input Processing
	/// </summary>
	virtual void ProcessInput(GLFWwindow* p_window, const float p_dt) = 0;

protected:
	SceneManager* m_sceneManager;
	EntityManager* m_entityManager;
	SystemManager* m_systemManager;
};


