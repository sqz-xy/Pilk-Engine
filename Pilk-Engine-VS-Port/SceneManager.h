#pragma once

#include "PilkEngineCommon.h"

class Scene;
enum SceneTypes;

// By Thomas Beet
class SceneManager final
{
	friend class Scene;
	friend class GameplayScene;
	friend class MainMenuScene;
	friend class BadEndingScene;
	friend class GoodEndingScene;
public:
	explicit SceneManager(int p_width, int p_height, const char* p_windowName);
	SceneManager(const SceneManager& p_sceneManager);
	~SceneManager();

	int Run();
	void ChangeScene(SceneTypes p_sceneType);

	SceneManager& operator=(const SceneManager& p_rhs);


private:
	void Render(float p_dt) const;
	void Update(float p_dt);
	void Load();

	void processInput(GLFWwindow* p_window, const float p_dt);
	
	const char* m_windowName = nullptr;
	Scene* m_currentScene;
	int m_width;
	int m_height;

};
