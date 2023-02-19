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
		ImVec2 vec(100, 50);
		//std::cout << "Rendering" << std::endl;

		ImGui::Begin("ImGui Test");
		ImGui::Text("Delta time %f", p_dt);
		bool button = ImGui::Button("Change scene", vec);

		if (button)
			m_sceneManager->ChangeScene(MainMenu);

		ImGui::End();
	}

	virtual void Update(const float p_dt) override
	{
		//std::cout << "Updating" << std::endl;
	}

	virtual void Load() override
	{
		std::cout << "Scene Loaded" << std::endl;
	}

	virtual void Close() override
	{
		std::cout << "Scene Closed" << std::endl;
	}
};