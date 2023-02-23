#pragma once

#include "../Managers/SceneManager.h"

class Scene
{
	friend class SceneManager;
public:
	explicit Scene(SceneManager *p_sceneManager);
	virtual ~Scene();

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
	/// Closing logic
	/// </summary>
	virtual void Close() = 0;

	/// <summary>
	/// Input logic
	/// </summary>
	virtual void processInput(GLFWwindow* p_window) = 0;

protected:
	SceneManager *m_sceneManager;
};


