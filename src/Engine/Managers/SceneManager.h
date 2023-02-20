#pragma once

#include "glad.h"
#include "glfw3.h"

#include "..\Scenes\Scene.h"
#include "../Scenes/SceneTypes.h"

class SceneManager
{
	friend class Scene;
	friend class MainMenuScene;
public:
	explicit SceneManager(int p_width, int p_height, char* p_windowName);
	SceneManager(const SceneManager& p_sceneManager);
	~SceneManager();

	int Run();
	void ChangeScene(SceneTypes p_sceneType);

	SceneManager& operator=(const SceneManager& p_rhs);


private:
	void Render(float p_dt) const;
	void Update(float p_dt);
	void Load();

	void processInput(GLFWwindow* p_window);

	Scene *m_currentScene;
	char* m_windowName = "";
	int m_width;
	int m_height;

};
