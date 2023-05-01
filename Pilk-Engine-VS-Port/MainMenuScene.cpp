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

	Timer* player1Timer;
	Timer* player2Timer;

	Camera* m_Camera;

	explicit MainMenuScene(SceneManager* pSceneManager) : Scene(pSceneManager)
	{
		m_sceneManager->m_windowName = "MainMenuScene";
		
		m_Camera = new Camera(	glm::vec3(3.75f,0.0f,21.50f),	// camPos
								glm::vec3(3.75f,0.0f,0.0f),  // camTarget
								m_sceneManager->m_width,	// windows width
								m_sceneManager->m_height);	// window height



		m_entityManager = new EntityManager();
		m_systemManager = new SystemManager();
		m_collisionManager = new GameCollisionManager();
		m_prefabManager = new PrefabManager();
		player1Timer = new Timer();
		player2Timer = new Timer();
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

		int count = m_entityManager->EntityCount();

		ImGui::Text("Entity Count %d", count);
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
		
		Entity* player1 = new Entity("Player1");
		player1->AddComponent(new ComponentTransform(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		player1->AddComponent(new ComponentCollisionAABB(1.0f, 1.0f, 0.5f));
		player1->AddComponent(new ComponentGeometry("resources/models/randy/randy.obj"));
		player1->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		player1->AddComponent(new ComponentPhysics(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.3f, 0.0f)));
		player1->AddComponent(new ComponentCollisionPoint(glm::vec3(0.0f, -1.1f, 0.0f)));
		player1->AddComponent(new ComponentProperties(false, 3.0f, 1.0f, vec3(1.0f, 0.0f, 0.0f)));

		Entity* player2 = new Entity("Player2");
		player2->AddComponent(new ComponentTransform(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		player2->AddComponent(new ComponentCollisionAABB(1.0f, 1.0f, 0.5f));
		player2->AddComponent(new ComponentGeometry("resources/models/randy/randy.obj"));
		player2->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		player2->AddComponent(new ComponentPhysics(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.3f, 0.0f)));
		player2->AddComponent(new ComponentCollisionPoint(glm::vec3(0.0f, -1.1f, 0.0f)));
		player2->AddComponent(new ComponentProperties(false, 3.0f, 1.0f, vec3(1.0f, 0.0f, 0.0f)));

		FileManager::LoadEntities("resources/scripts/EntityScript.txt");

		m_entityManager->AddEntity(player1);
		m_entityManager->AddEntity(player2);

		// System render
		System* systemRender = new SystemRender(m_Camera);
		System* systemPhysics = new SystemPhysics();
		System* system_collision_AABB_AABB = new SystemCollisionAABBAABB(m_collisionManager);
		System* system_collision_aabb_point = new SystemCollisionAABBPoint(m_collisionManager);

		m_systemManager->AddSystem(systemRender);
		m_systemManager->AddSystem(systemPhysics);
		m_systemManager->AddSystem(system_collision_AABB_AABB);
		m_systemManager->AddSystem(system_collision_aabb_point);

		//m_entityManager->ValidateEntities(m_systemManager);
		m_prefabManager->RegisterLevel(*m_entityManager, *m_systemManager);	
	}

	void Shoot(Entity* p_player)
	{
		glm::vec3 pos = p_player->GetComponent<ComponentTransform>()->m_translation;
		glm::vec3 dir = p_player->GetComponent<ComponentProperties>()->m_direction;
		float dmg = p_player->GetComponent<ComponentProperties>()->m_damage;

		Entity* bullet = new Entity("Bullet");
		bullet->AddComponent(new ComponentTransform(pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		bullet->AddComponent(new ComponentPhysics(dir * 20.0f, glm::vec3(0.0f, 0.0f, 0.0f)));
		bullet->AddComponent(new ComponentProperties(true, 1.0f, dmg, dir));
		bullet->AddComponent(new ComponentGeometry("resources/models/randy/randy.obj"));
		bullet->AddComponent(new ComponentShader("resources/shaders/Billboard.vert", "resources/shaders/FireShader.frag"));

		m_entityManager->AddEntity(bullet);
		m_systemManager->ValidateEntity(bullet);
		//m_entityManager->ValidateEntities(m_systemManager);
	}

	void ProcessInput(GLFWwindow* p_window, const float p_dt) override
	{
		// get the players from the list
		Entity* player1 = m_entityManager->FindEntity("Player1");
		Entity* player2 = m_entityManager->FindEntity("Player2");

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

		// player control below here
		MovePlayerOne(p_window, p_dt, player1);
		MovePlayerTwo(p_window, p_dt, player2);

#if MOUSE_CONTROL_ENABLED
		double xpos, ypos;
		glfwGetCursorPos(p_window, &xpos, &ypos);
		glm::vec2 pos(xpos, ypos);
		m_Camera->RotateCamera(pos);
#endif		
	}

	void MovePlayerOne(GLFWwindow* p_window, const float p_dt, Entity* p_player)
	{
		ComponentProperties* prop = p_player->GetComponent<ComponentProperties>();
		if (glfwGetKey(p_window, GLFW_KEY_UP) == GLFW_PRESS && prop->m_hasJumped == false)
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			phys->SetVelY(10.0f);
			prop->m_hasJumped = true;
		}

		if (glfwGetKey(p_window, GLFW_KEY_ENTER) == GLFW_PRESS)
		{
			if (player1Timer->GetElapsedTime() > 0.25f)
			{
				std::cout << player1Timer->GetElapsedTime() << std::endl;
				Shoot(p_player);
				player1Timer->Restart();
			}
		}

		if (glfwGetKey(p_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			ComponentProperties* prop = p_player->GetComponent<ComponentProperties>();
			prop->m_direction = glm::vec3(1.0f, 0.0f, 0.0f);
			phys->SetVelX(5.0f);
		}
		else if (glfwGetKey(p_window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			ComponentProperties* prop = p_player->GetComponent<ComponentProperties>();
			prop->m_direction = glm::vec3(-1.0f, 0.0f, 0.0f);
			phys->SetVelX(-5.0f);
		}
		else
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			phys->SetVelX(0.0f);
		}


	}
	void MovePlayerTwo(GLFWwindow* p_window, const float p_dt, Entity* p_player)
	{
		ComponentProperties* prop = p_player->GetComponent<ComponentProperties>();
		if (glfwGetKey(p_window, GLFW_KEY_W) == GLFW_PRESS && prop->m_hasJumped == false)
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			phys->SetVelY(10.0f);
			prop->m_hasJumped = true;
		}

		if (glfwGetKey(p_window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if (player2Timer->GetElapsedTime() > 0.25f)
			{
				std::cout << player2Timer->GetElapsedTime() << std::endl;
				Shoot(p_player);
				player2Timer->Restart();
			}
		}

		if (glfwGetKey(p_window, GLFW_KEY_D) == GLFW_PRESS)
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			ComponentProperties* prop = p_player->GetComponent<ComponentProperties>();
			prop->m_direction = glm::vec3(1.0f, 0.0f, 0.0f);
			phys->SetVelX(5.0f);
		}
		else if (glfwGetKey(p_window, GLFW_KEY_A) == GLFW_PRESS)
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			ComponentProperties* prop = p_player->GetComponent<ComponentProperties>();
			prop->m_direction = glm::vec3(-1.0f, 0.0f, 0.0f);
			phys->SetVelX(-5.0f);
		}
		else
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			phys->SetVelX(0.0f);
		}
	}

    void Close() override
	{
		Scene::Close();

		std::cout << "Scene Closed" << std::endl;

		delete m_Camera;

		ResourceManager::DeleteResources();
	}
};