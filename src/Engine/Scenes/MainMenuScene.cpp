#pragma once

#include "Scene.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/EntityManager.h"
#include "../Managers/FileManager.h"
#include "../Utility/Camera.h"	
#include "../PilkEngineCommon.h"
#include "../Objects/Geometry.h"
#include "../Objects/Model.h"
#include "../Components/Component.h"
#include "../Objects/Entity.h"


class MainMenuScene : public Scene
{
public:

	EntityManager* m_entityManager;
	SystemManager* m_systemManager;

	Camera* m_Camera;

	explicit MainMenuScene(SceneManager* pSceneManager) : Scene(pSceneManager)
	{
		m_sceneManager->m_windowName = "MainMenuScene";
		
		m_Camera = new Camera(	glm::vec3(0.0f,0.0f,15.0f),	// camPos
								glm::vec3(0.0f,0.0f,-5.0f),  // camTarget
								m_sceneManager->m_width,	// windows width
								m_sceneManager->m_height);	// window height



		m_entityManager = new EntityManager();
		m_systemManager = new SystemManager();
	}


	~MainMenuScene() override
	{
		this->Close();
	}

	void Render(const float p_dt) const override
	{
		ImVec2 vec(100, 50);
		//std::cout << "Rendering" << std::endl;
		ImGui::Begin("ImGui Test");
		ImGui::Text("Delta time %f", p_dt);
		bool button = ImGui::Button("Change scene", vec);

		if (button)
			m_sceneManager->ChangeScene(MainMenu);

		glUseProgram(0);

		ImGui::End();
	}

	void Update(const float p_dt) override
	{
		m_Camera->UpdateCamera();

		m_systemManager->ExecuteSystems(p_dt);
	}

	void Load() override
	{
		stbi_set_flip_vertically_on_load(true);

		// Player entity.
		Entity* player = new Entity("Player");
		player->AddComponent(new ComponentTransform(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		player->AddComponent(new ComponentGeometry("resources/models/randy/randy.obj"));
		player->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		player->AddComponent(new ComponentPhysics(glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.0f, -0.01f, 0.0f)));

		m_entityManager->AddEntity(player);

		// System render
		System* systemRender = new SystemRender(m_Camera);
		systemRender->ValidateEntity(player);
		System* systemPhysics = new SystemPhysics();
		systemPhysics->ValidateEntity(player);

		m_systemManager->AddSystem(systemRender);
		m_systemManager->AddSystem(systemPhysics);
	}

	void ProcessInput(GLFWwindow* p_window, const float p_dt) override
	{
		// Exit if escape key is pressed
		if (glfwGetKey(p_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(p_window, true);

		if (glfwGetKey(p_window, GLFW_KEY_W) == GLFW_PRESS)
			m_Camera->MoveCamera(m_Camera->Forward, 2.5f, p_dt);

		if (glfwGetKey(p_window, GLFW_KEY_S) == GLFW_PRESS)
			m_Camera->MoveCamera(m_Camera->Backward, 2.5f, p_dt);

		if (glfwGetKey(p_window, GLFW_KEY_A) == GLFW_PRESS)
			m_Camera->MoveCamera(m_Camera->Left, 2.5f, p_dt);

		if (glfwGetKey(p_window, GLFW_KEY_D) == GLFW_PRESS)
			m_Camera->MoveCamera(m_Camera->Right, 2.5f, p_dt);

#if MOUSE_CONTROL_ENABLED
		double xpos, ypos;
		glfwGetCursorPos(p_window, &xpos, &ypos);
		m_Camera->RotateCamera(glm::vec2(xpos, ypos));
#endif		
	}

    void Close() override
	{
		Scene::Close();

		std::cout << "Scene Closed" << std::endl;

		delete m_Camera;

		ResourceManager::DeleteResources();
	}
};