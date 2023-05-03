#pragma once

#include "PilkEngineCommon.h"

#include "GameCollisionManager.cpp"
#include "Scene.h"
#include "PrefabManager.h"
#include "Timer.h"
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "SceneManager.h"
#include "FileManager.h"
#include "SceneTypes.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "SystemManager.h"

class MainMenuScene : public Scene
{
public:

	EntityManager* m_entityManager;
	SystemManager* m_systemManager;
	GameCollisionManager* m_collisionManager;
	PrefabManager* m_prefabManager;

	Timer* player1Timer;
	Timer* player2Timer;

	Timer* modelTimer;
	int texIndex = 1;

	Camera* m_Camera;

	float m_levelWidth;

	explicit MainMenuScene(SceneManager* pSceneManager) : Scene(pSceneManager)
	{
		m_sceneManager->m_windowName = "MainMenuScene";
		
		m_Camera = new Camera(	glm::vec3(3.75f,0.0f,21.50f),	// camPos
								glm::vec3(3.75f,0.0f,0.0f),  // camTarget
								m_sceneManager->m_width,	// windows width
								m_sceneManager->m_height);	// window height



		m_entityManager = new EntityManager();
		m_systemManager = new SystemManager();
		m_collisionManager = new GameCollisionManager(m_systemManager, m_entityManager);
		m_prefabManager = new PrefabManager();
		player1Timer = new Timer();
		player2Timer = new Timer();
		modelTimer = new Timer();
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
		glm::vec3 movementRate = glm::vec3(1.0f, 0.0f, 0.0f) * p_dt;

		if (m_Camera->m_cameraPos.x < m_levelWidth)
		{
			m_Camera->m_cameraPos += movementRate;
		}
		else
		{
			movementRate = glm::vec3(0.0f);
		}

		for (auto entity : m_entityManager->m_entities)
		{
			ComponentTransform* trans = entity->GetComponent<ComponentTransform>();
			if (trans != nullptr)
			{
				if (trans->m_translation.z > 10.0f)
				{
					m_systemManager->RemoveEntity(entity);
					m_entityManager->RemoveEntity(entity->GetID());
				}

				if (entity->GetName() == "Player1" || entity->GetName() == "Player2")
				{
					if (trans->m_translation.x < m_Camera->m_cameraPos.x - 10.0f)
					{
						m_systemManager->RemoveEntity(entity);
						m_entityManager->RemoveEntity(entity->GetID());
						entity->Delete(true);
					}
				}

				if (entity->GetName() == "P1Heart" || entity->GetName() == "P2Heart")
				{		
					trans->m_translation += movementRate;
					trans->UpdateTranslation(trans->m_translation);
				}
			}

			ComponentProperties* prop = entity->GetComponent<ComponentProperties>();

			if (prop != nullptr)
			{
				if (prop->m_health <= 0.0f)
				{
					if (entity->GetName() == "Player1" || entity->GetName() == "Player2")
					{
						m_systemManager->RemoveEntity(entity);
						m_entityManager->RemoveEntity(entity->GetID());
						entity->Delete(true);
					}
				}
			}
		}

		m_Camera->UpdateCamera();


		m_systemManager->ExecuteSystems(p_dt);

		m_collisionManager->ProcessCollisions();
		m_collisionManager->ClearManifold();

		for (int i = 0; i < m_entityManager->m_entities.size(); i++)
		{
			if (m_entityManager->m_entities[i]->IsDelete())
			{
				m_systemManager->RemoveEntity(m_entityManager->m_entities[i]);
				m_entityManager->RemoveEntity(m_entityManager->m_entities[i]->GetID());
			}
		}
	}

	void Load() override
	{

		m_prefabManager->LoadPrefabs("resources/prefabs/Level1", "");

		//stbi_set_flip_vertically_on_load(true);

		// Hearts

		Entity* heart1 = new Entity("P1Heart");
		heart1->AddComponent(new ComponentTransform(glm::vec3(5.0f, 3.0f, 10.0f), glm::vec3(0.0f, 1.5708f, 0.0f), glm::vec3(0.25f, 0.25f, 1.0f)));
		heart1->AddComponent(new ComponentGeometry("resources/models/heart/heart.obj"));
		heart1->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		m_entityManager->AddEntity(heart1);

		Entity* heart2 = new Entity("P1Heart");
		heart2->AddComponent(new ComponentTransform(glm::vec3(6.5f, 3.0f, 10.0f), glm::vec3(0.0f, 1.5708f, 0.0f), glm::vec3(0.25f, 0.25f, 1.0f)));
		heart2->AddComponent(new ComponentGeometry("resources/models/heart/heart.obj"));
		heart2->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		m_entityManager->AddEntity(heart2);

		Entity* heart3 = new Entity("P1Heart");
		heart3->AddComponent(new ComponentTransform(glm::vec3(8.0f, 3.0f, 10.0f), glm::vec3(0.0f, 1.5708f, 0.0f), glm::vec3(0.25f, 0.25f, 1.0f)));
		heart3->AddComponent(new ComponentGeometry("resources/models/heart/heart.obj"));
		heart3->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		m_entityManager->AddEntity(heart3);

		Entity* heart4 = new Entity("P2Heart");
		heart4->AddComponent(new ComponentTransform(glm::vec3(-0.5f, 3.0f, 10.0f), glm::vec3(0.0f, 1.5708f, 0.0f), glm::vec3(0.25f, 0.25f, 1.0f)));
		heart4->AddComponent(new ComponentGeometry("resources/models/heart/heart.obj"));
		heart4->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		m_entityManager->AddEntity(heart4);

		Entity* heart5 = new Entity("P2Heart");
		heart5->AddComponent(new ComponentTransform(glm::vec3(1.0f, 3.0f, 10.0f), glm::vec3(0.0f, 1.5708f, 0.0f), glm::vec3(0.25f, 0.25f, 1.0f)));
		heart5->AddComponent(new ComponentGeometry("resources/models/heart/heart.obj"));
		heart5->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		m_entityManager->AddEntity(heart5);

		Entity* heart6 = new Entity("P2Heart");
		heart6->AddComponent(new ComponentTransform(glm::vec3(2.5f, 3.0f, 10.0f), glm::vec3(0.0f, 1.5708f, 0.0f), glm::vec3(0.25f, 0.25f, 1.0f)));
		heart6->AddComponent(new ComponentGeometry("resources/models/heart/heart.obj"));
		heart6->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		m_entityManager->AddEntity(heart6);

		// Player entity.
		
		Entity* player1 = new Entity("Player1");
		player1->AddComponent(new ComponentTransform(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.5708f, 3.14159f, 0.0f), glm::vec3(0.8f, 0.8f, 1.0f)));
		player1->AddComponent(new ComponentCollisionAABB(1.3f, 1.0f, 0.5f));
		player1->AddComponent(new ComponentGeometry("resources/models/player1/player01.obj"));
		player1->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		player1->AddComponent(new ComponentPhysics(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.3f, 0.0f)));
		player1->AddComponent(new ComponentCollisionPoint(glm::vec3(0.0f, -1.4f, 0.0f)));
		player1->AddComponent(new ComponentCollisionPoint(glm::vec3(0.5f, -1.4f, 0.0f)));
		player1->AddComponent(new ComponentProperties(false, 3.0f, 1.0f, vec3(1.0f, 0.0f, 0.0f)));

		Entity* player2 = new Entity("Player2");
		player2->AddComponent(new ComponentTransform(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(1.5708f, 3.14159f, 0.0f), glm::vec3(0.8f, 0.8f, 1.0f)));
		player2->AddComponent(new ComponentCollisionAABB(1.3f, 1.0f, 0.5f));
		player2->AddComponent(new ComponentGeometry("resources/models/player1/player01.obj"));
		player2->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		player2->AddComponent(new ComponentPhysics(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.3f, 0.0f)));
		player2->AddComponent(new ComponentCollisionPoint(glm::vec3(0.0f, -1.4f, 0.0f)));
		player2->AddComponent(new ComponentProperties(false, 3.0f, 1.0f, vec3(1.0f, 0.0f, 0.0f)));

		FileManager::LoadEntities("resources/scripts/EntityScript.txt");

		m_entityManager->AddEntity(player1);
		m_entityManager->AddEntity(player2);

		// System render
		System* systemRender = new SystemRender(m_Camera);
		System* systemPhysics = new SystemPhysics();
		System* system_collision_AABB_AABB = new SystemCollisionAABBAABB(m_collisionManager);
		System* system_collision_aabb_point = new SystemCollisionAABBPoint(m_collisionManager);
		System* system_collision_sphere_point = new SystemCollisionSpherePoint(m_collisionManager);

		m_systemManager->AddSystem(systemRender);
		m_systemManager->AddSystem(systemPhysics);
		m_systemManager->AddSystem(system_collision_AABB_AABB);
		m_systemManager->AddSystem(system_collision_aabb_point);
		m_systemManager->AddSystem(system_collision_sphere_point);

		//m_entityManager->ValidateEntities(m_systemManager);
		Level lvl = m_prefabManager->RegisterLevel(*m_entityManager, *m_systemManager);	
		m_levelWidth = lvl.width;
	}

	void Shoot(Entity* p_player)
	{
		glm::vec3 pos = p_player->GetComponent<ComponentTransform>()->m_translation;
		glm::vec3 dir = p_player->GetComponent<ComponentProperties>()->m_direction;
		float dmg = p_player->GetComponent<ComponentProperties>()->m_damage;

		pos.x += dir.x;

		Entity* bullet = new Entity("Bullet");
		bullet->AddComponent(new ComponentTransform(pos, glm::vec3(2.356194f, 3.14159f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		bullet->AddComponent(new ComponentPhysics(dir * 20.0f, glm::vec3(0.0f, 0.0f, 0.0f)));
		bullet->AddComponent(new ComponentCollisionAABB(1.0f, 1.0f, 1.0f));
		bullet->AddComponent(new ComponentProperties(true, 1.0f, dmg, dir));
		bullet->AddComponent(new ComponentGeometry("resources/models/torch/fire02.obj"));
		bullet->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FireShader.frag"));

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
		if (player1 != nullptr)
			MovePlayerOne(p_window, p_dt, player1);

		if (player2 != nullptr)
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
		GLFWgamepadstate state;
		int success = glfwGetGamepadState(0, &state);

		ComponentProperties* prop = p_player->GetComponent<ComponentProperties>();
		if ((glfwGetKey(p_window, GLFW_KEY_UP) == GLFW_PRESS || (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] && success)) || (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] && success) && prop->m_hasJumped == false)
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			phys->SetVelY(10.0f);
			prop->m_hasJumped = true;
		}

		if (glfwGetKey(p_window, GLFW_KEY_ENTER) == GLFW_PRESS || (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] && success))
		{
			if (player1Timer->GetElapsedTime() > 0.25f)
			{
				std::cout << player1Timer->GetElapsedTime() << std::endl;
				Shoot(p_player);
				player1Timer->Restart();
			}
		}

		if (glfwGetKey(p_window, GLFW_KEY_RIGHT) == GLFW_PRESS || (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] && success))
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			ComponentProperties* prop = p_player->GetComponent<ComponentProperties>();
			prop->m_direction = glm::vec3(1.0f, 0.0f, 0.0f);
			phys->SetVelX(5.0f);

			ComponentTransform* pos1 = p_player->GetComponent<ComponentTransform>();
			vec3 newscale = pos1->m_scale;
			newscale.x = 0.8f;
			pos1->UpdateScale(newscale);
		}
		else if (glfwGetKey(p_window, GLFW_KEY_LEFT) == GLFW_PRESS || (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] && success))
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			ComponentProperties* prop = p_player->GetComponent<ComponentProperties>();
			prop->m_direction = glm::vec3(-1.0f, 0.0f, 0.0f);
			phys->SetVelX(-5.0f);

			ComponentTransform* pos1 = p_player->GetComponent<ComponentTransform>();
			vec3 newscale = pos1->m_scale;
			newscale.x = -0.8f;
			pos1->UpdateScale(newscale);
		}
		else
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			phys->SetVelX(0.0f);
		}


	}
	void MovePlayerTwo(GLFWwindow* p_window, const float p_dt, Entity* p_player)
	{
		GLFWgamepadstate state;
		int success = glfwGetGamepadState(0, &state);

		ComponentProperties* prop = p_player->GetComponent<ComponentProperties>();
		if ((glfwGetKey(p_window, GLFW_KEY_W) == GLFW_PRESS || (state.buttons[GLFW_GAMEPAD_BUTTON_A] && success)) || (state.buttons[GLFW_GAMEPAD_BUTTON_Y] && success) && prop->m_hasJumped == false)
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			phys->SetVelY(10.0f);
			prop->m_hasJumped = true;
		}

		if (glfwGetKey(p_window, GLFW_KEY_SPACE) == GLFW_PRESS || (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] && success))
		{
			if (player2Timer->GetElapsedTime() > 0.25f)
			{
				std::cout << player2Timer->GetElapsedTime() << std::endl;
				Shoot(p_player);
				player2Timer->Restart();
			}
		}

		if (glfwGetKey(p_window, GLFW_KEY_D) == GLFW_PRESS || (state.buttons[GLFW_GAMEPAD_BUTTON_B] && success))
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			ComponentProperties* prop = p_player->GetComponent<ComponentProperties>();
			prop->m_direction = glm::vec3(1.0f, 0.0f, 0.0f);
			phys->SetVelX(5.0f);

			ComponentTransform* pos1 = p_player->GetComponent<ComponentTransform>();
			vec3 newscale = pos1->m_scale;
			newscale.x = 0.8f;
			pos1->UpdateScale(newscale);
		}
		else if (glfwGetKey(p_window, GLFW_KEY_A) == GLFW_PRESS || (state.buttons[GLFW_GAMEPAD_BUTTON_X] && success))
		{
			ComponentPhysics* phys = p_player->GetComponent<ComponentPhysics>();
			ComponentProperties* prop = p_player->GetComponent<ComponentProperties>();
			prop->m_direction = glm::vec3(-1.0f, 0.0f, 0.0f);
			phys->SetVelX(-5.0f);

			ComponentTransform* pos1 = p_player->GetComponent<ComponentTransform>();
			vec3 newscale = pos1->m_scale;
			newscale.x = -0.8f;
			pos1->UpdateScale(newscale);
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