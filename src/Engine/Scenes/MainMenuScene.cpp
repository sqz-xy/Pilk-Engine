#pragma once
#include "Scene.h"
#include <iostream>
#include <imgui_impl_opengl3.h>

class MainMenuScene : public Scene
{
public:
	explicit MainMenuScene(SceneManager* pSceneManager) : Scene(pSceneManager)
	{
		m_sceneManager->m_windowName = "MainMenuScene";
	}

	virtual ~MainMenuScene() override 
	{
		this->Close();
	}

	virtual void Render(const float p_dt) const override
	{
		std::cout << "Rendering" << std::endl;

		ImGui::Begin("ImGui Test");
		ImGui::Text("%d", p_dt);
		ImGui::End();
	}

	virtual void Update(const float p_dt) override
	{
		std::cout << "Updating" << std::endl;
	}

	virtual void Load() override
	{
		std::cout << "Loaded" << std::endl;
	}

	virtual void Close() override
	{
		std::cout << "Closed" << std::endl;
	}
};