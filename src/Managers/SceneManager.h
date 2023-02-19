#pragma once

#include "glad.h"
#include "glfw3.h"

class SceneManager
{
public:
	explicit SceneManager(const int pWidth, const int pHeight, char* pWindowName);
	explicit SceneManager(const SceneManager& pSceneManager);
	~SceneManager(void);

	int Run();


private:
	void Render(const float pDt) const;
	void Update(const float pDt);
	void Load();

	void processInput(GLFWwindow* window);

	char* m_windowName = "";
	int m_width;
	int m_height;

};
