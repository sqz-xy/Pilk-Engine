#pragma once

#include "PilkEngineCommon.h"
#include "GameCollisionManager.cpp"

class MainMenuScene : public Scene
{
public:

	EntityManager* m_entityManager;
	SystemManager* m_systemManager;
	GameCollisionManager* m_collisionManager;
	PrefabManager* m_prefabManager;

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
		m_collisionManager = new GameCollisionManager();
		m_prefabManager = new PrefabManager();
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

		m_collisionManager->ProcessCollisions();
		m_collisionManager->ClearManifold();
	}

	void Load() override
	{
		m_prefabManager->LoadPrefabs("resources/prefabs/Level1", "");

		stbi_set_flip_vertically_on_load(true);

		// Player entity.
		
		Entity* player = new Entity("Player");
		player->AddComponent(new ComponentTransform(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		player->AddComponent(new ComponentCollisionAABB(2.0f, 2.0f, 0.5f));
		player->AddComponent(new ComponentGeometry("resources/models/randy/randy.obj"));
		player->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		player->AddComponent(new ComponentPhysics(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.01f, 0.0f)));

		Entity* floor = new Entity("Floor");
		floor->AddComponent(new ComponentTransform(glm::vec3(0.0f, -3.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		floor->AddComponent(new ComponentCollisionAABB(2.0f, 2.0f, 0.5f));
		floor->AddComponent(new ComponentGeometry("resources/models/tempcube/tempcube.obj"));
		floor->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		floor->AddComponent(new ComponentPhysics(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

		//Entity* floor2 = new Entity("Floor2");
		//floor2->AddComponent(new ComponentTransform(glm::vec3(2.0f, -3.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		//floor2->AddComponent(new ComponentGeometry("resources/models/tempcube/tempcube.obj"));
		//floor2->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		//floor2->AddComponent(new ComponentPhysics(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

		//FileManager::LoadEntities("resources/scripts/EntityScript.txt");

		m_entityManager->AddEntity(player);
		m_entityManager->AddEntity(floor);
		//m_entityManager->AddEntity(floor2);

		// System render
		System* systemRender = new SystemRender(m_Camera);
		System* systemPhysics = new SystemPhysics();
		System* system_collision_AABB_AABB = new SystemCollisionAABBAABB(m_collisionManager);

		m_systemManager->AddSystem(systemRender);
		m_systemManager->AddSystem(systemPhysics);
		m_systemManager->AddSystem(system_collision_AABB_AABB);

		m_entityManager->ValidateEntities(m_systemManager);
		
	}

	void ProcessInput(GLFWwindow* p_window, const float p_dt) override
	{
		// uh oh! this is temp!
		Entity* playor = m_entityManager->FindEntity("Player");

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

		if (glfwGetKey(p_window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			ComponentPhysics* phys = playor->GetComponent<ComponentPhysics>();
			phys->SetVelocity(vec3(0,10.0,0));
		}

		if (glfwGetKey(p_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			ComponentPhysics* phys = playor->GetComponent<ComponentPhysics>();
			phys->SetVelocity(vec3(10.0, 0, 0));
		}

		if (glfwGetKey(p_window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			ComponentPhysics* phys = playor->GetComponent<ComponentPhysics>();
			phys->SetVelocity(vec3(-10.0, 0, 0));
		}

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