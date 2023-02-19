#pragma once

#include "glad.h"
#pragma once
#include "glfw3.h"
#include "..\Scenes\Scene.h"
#include "../Scenes/SceneTypes.h"
#include <functional>

class SceneManager
{
	friend class Scene;
	friend class MainMenuScene;
public:
	explicit SceneManager(const int p_width, const int p_height, char* p_windowName);
	explicit SceneManager(const SceneManager& p_sceneManager);
	~SceneManager(void);

	int Run();
	void ChangeScene(SceneTypes p_sceneType);

	SceneManager& operator=(const SceneManager& p_rhs);


private:
	void Render(const float p_dt) const;
	void Update(const float p_dt);
	void Load();

	void processInput(GLFWwindow* p_window);

	Scene *m_currentScene;
	char* m_windowName = "";
	int m_width;
	int m_height;

};
