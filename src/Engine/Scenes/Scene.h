#pragma once
#include "../Managers/SceneManager.h"

class Scene
{
	friend class SceneManager;
public:
	explicit Scene(SceneManager *p_sceneManager);
	virtual ~Scene(void);

	virtual void Render(const float p_dt) const = 0;
	virtual void Update(const float p_dt) = 0;
	virtual void Load() = 0;
	virtual void Close() = 0;

protected:
	SceneManager *m_sceneManager;
};

