#include "PilkEngineCommon.h"
#include "PrefabManager.h"

PrefabManager::PrefabManager()
{
}

void PrefabManager::LoadPrefabs(const std::string& p_prefabPath, const std::string& p_entityScriptPath)
{

	glm::vec3 levelStartPos(0.0f, 0.0f, 0.0f);

	for (const auto& prefabFilePath : std::filesystem::directory_iterator(p_prefabPath))
	{
		std::ifstream prefabFile(prefabFilePath.path());

		if (!prefabFile)
			return;

		Prefab* prefab = new Prefab();
		int lineIndex = 0;
		int yOffset = PREFAB_SIZE;

		while (!prefabFile.eof())
		{
			std::string line;
			prefabFile >> line;

			for (int i = 0; i < line.length(); i++)
			{
				// block
				if (line[i] == '1')
				{
					Entity* block = new Entity("Block");

					float xPos = ((i * 2) - (PREFAB_SIZE - 1)) + levelStartPos.x;
					float yPos = ((yOffset * 2) - (PREFAB_SIZE - 1)) - 2.0f;

					block->AddComponent(new ComponentTransform(glm::vec3(xPos, yPos, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.5f, 1.0f)));
					block->AddComponent(new ComponentCollisionAABB(2.0f, 2.0f, 0.5f));
					block->AddComponent(new ComponentGeometry("resources/models/tempcube/tempcube.obj"));
					block->AddComponent(new ComponentShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag"));

					prefab->Entities[lineIndex][i] = block;
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
