#pragma once

#include "PilkEngineCommon.h"

#include "GameCollisionManager.cpp"
#include "Scene.h"
#include "Entity.h"
#include "System.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "SceneTypes.h"

class BadEndingScene : public Scene
{
public:
	Camera* m_Camera;

	explicit BadEndingScene(SceneManager* pSceneManager) : Scene(pSceneManager)
	{
		m_sceneManager->m_windowName = "Bad Ending!";

		glm::vec3 camPos = glm::vec3(0.0f, 0.0f, -2.0f);
		glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, 0.0f);

		m_Camera = new Camera(glm::vec3(3.75f, 0.0f, 21.50f),	// camPos
			glm::vec3(3.75f, 0.0f, 0.0f),  // camTarget
			m_sceneManager->m_width,	// windows width
			m_sceneManager->m_height);	// window height
	}

	~BadEndingScene() override
	{
		this->Close();
	}



	void Render(const float p_dt) const override
	{
		ImVec2 vec(100, 50);
		//std::cout << "Rendering" << std::endl;
		ImGui::Begin("ImGui Test");
		ImGui::Text("Delta time %f", p_dt);

		int count = m_entityManager->EntityCount();

		ImGui::Text("Entity Count %d", count);
		bool button = ImGui::Button("Change scene", vec);

		if (button)
			m_sceneManager->ChangeScene(Gameplay);

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
		// Player entity.

		Entity* player1 = new Entity("Player1");
		player1->AddComponent(new ComponentTransform(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.5708f, 3.14159f, 0.0f), glm::vec3(0.8f, 0.8f, 1.0f)));
		player1->AddComponent(new ComponentGeometry("resources/models/player1/player01.obj"));
		player1->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));

		m_entityManager->AddEntity(player1);

		// System render
		System* systemRender = new SystemRender(m_Camera);
		m_systemManager->AddSystem(systemRender);
		m_entityManager->ValidateEntities(m_systemManager);
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
		glm::vec2 pos(xpos, ypos);
		m_Camera->RotateCamera(pos);
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