#include "PilkEngineCommon.h"
#include "PrefabManager.h"

PrefabManager::PrefabManager()
{
}

void PrefabManager::LoadPrefabs(const std::string& p_prefabPath, const std::string& p_entityScriptPath)
{

	glm::vec3 levelStartPos(0.0f, 0.0f, 5.0f);

	for (const auto& prefabFilePath : std::filesystem::directory_iterator(p_prefabPath))
	{
		std::ifstream prefabFile(prefabFilePath.path());

		if (!prefabFile)
			return;

		Prefab* prefab = new Prefab();
		int lineIndex = 0;
		int yOffset = PREFAB_SIZE;

		// Extra Geometry
		glm::vec3 backwallPos = levelStartPos;
		backwallPos.z -= 2.0f;

		Entity* backwall = new Entity("Backwall");
		backwall->AddComponent(new ComponentTransform(backwallPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(6.0f, 6.0f, 1.0f)));
		backwall->AddComponent(new ComponentGeometry("resources/models/tempcube/tempcube.obj"));
		backwall->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		prefab->BackWall = backwall;

		glm::vec3 ceilingPos = levelStartPos;
		ceilingPos.y += 7;

		Entity* ceiling = new Entity("Ceiling");
		ceiling->AddComponent(new ComponentTransform(ceilingPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(6.0f, 1.0f, 1.0f)));
		ceiling->AddComponent(new ComponentGeometry("resources/models/tempcube/tempcube.obj"));
		ceiling->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		ceiling->AddComponent(new ComponentCollisionAABB(2.0f, 12.0f, 0.0f));
		prefab->Ceiling = ceiling;

		glm::vec3 floorPos = levelStartPos;
		floorPos.y -= 7;

		Entity* floor = new Entity("Floor");
		floor->AddComponent(new ComponentTransform(floorPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(6.0f, 1.0f, 1.0f)));
		floor->AddComponent(new ComponentGeometry("resources/models/tempcube/tempcube.obj"));
		floor->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
		floor->AddComponent(new ComponentCollisionAABB(1.0f, 12.0f, 0.0f));
		prefab->Floor = floor;
		//END

		while (!prefabFile.eof())
		{
			std::string line;
			prefabFile >> line;

			for (int i = 0; i < line.length(); i++)
			{
				float xPos = ((i * 2) - (PREFAB_SIZE - 1)) + levelStartPos.x;
				float yPos = ((yOffset * 2) - (PREFAB_SIZE - 1)) - 2.0f;

				// block
				if (line[i] == '1')
				{
					Entity* block = new Entity("Block");

					block->AddComponent(new ComponentTransform(glm::vec3(xPos, yPos, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
					block->AddComponent(new ComponentCollisionAABB(2.0f, 2.0f, 0.0f));
					block->AddComponent(new ComponentGeometry("resources/models/tempcube/tempcube.obj"));
					block->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));

					prefab->Entities[lineIndex][i] = block;
				}

				// Enemy
				if (line[i] == 'E')
				{
					Entity* enemy = new Entity("GroundEnemy");

					enemy->AddComponent(new ComponentTransform(glm::vec3(xPos, yPos, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
					enemy->AddComponent(new ComponentCollisionAABB(1.0f, 1.0f, 0.5f));
					enemy->AddComponent(new ComponentGeometry("resources/models/fire/fire.obj"));
					enemy->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FireShader.frag"));
					enemy->AddComponent(new ComponentPhysics(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.3f, 0.0f)));
					enemy->AddComponent(new ComponentCollisionPoint(glm::vec3(0.0f, -1.1f, 0.0f)));
					enemy->AddComponent(new ComponentProperties(false, 3.0f, 3.0f, glm::vec3(0.0f, 0.0f, 0.0f)));

					prefab->Entities[lineIndex][i] = enemy;
				}

				// Enemy
				if (line[i] == 'F')
				{
					Entity* enemy = new Entity("FlyingEnemy");

					enemy->AddComponent(new ComponentTransform(glm::vec3(xPos, yPos, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
					enemy->AddComponent(new ComponentCollisionAABB(1.0f, 1.0f, 0.5f));
					enemy->AddComponent(new ComponentGeometry("resources/models/fire/fire.obj"));
					enemy->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));
					enemy->AddComponent(new ComponentPhysics(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
					enemy->AddComponent(new ComponentCollisionPoint(glm::vec3(0.0f, -1.1f, 0.0f)));
					enemy->AddComponent(new ComponentCollisionSphere(1.0f));
					enemy->AddComponent(new ComponentProperties(false, 3.0f, 3.0f, glm::vec3(0.0f, 0.0f, 0.0f)));

					prefab->Entities[lineIndex][i] = enemy;
				}
			}

			lineIndex++;
			yOffset--;
		}
		m_level.push_back(prefab);
		levelStartPos.x += PREFAB_SIZE * 2.0f;
	}
}

std::vector<Prefab*> PrefabManager::RegisterLevel(EntityManager& p_entityManager, SystemManager& p_systemManager)
{
	for (auto& prefab : m_level)
	{

		p_entityManager.AddEntity(prefab->BackWall);
		p_entityManager.AddEntity(prefab->Floor);
		p_entityManager.AddEntity(prefab->Ceiling);

		for (int i = 0; i < PREFAB_SIZE; i++)
			for (int j = 0; j < PREFAB_SIZE; j++)
			{
				if (prefab->Entities[i][j] == nullptr)
					continue;

				p_entityManager.AddEntity(prefab->Entities[i][j]);
			}
	}
	p_entityManager.ValidateEntities(&p_systemManager);
	return m_level;
}
