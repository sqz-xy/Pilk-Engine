#include "PilkEngineCommon.h"
#include "PrefabManager.h"

PrefabManager::PrefabManager()
{
}

void PrefabManager::LoadPrefabs(const std::string& p_prefabPath, const std::string& p_entityScriptPath)
{
	for (const auto& prefabFilePath : std::filesystem::directory_iterator(p_prefabPath))
	{
		std::ifstream prefabFile(prefabFilePath.path());

		if (!prefabFile)
			return;

		Prefab* prefab = new Prefab();
		int lineIndex = 0;

		while (!prefabFile.eof())
		{
			std::string line;
			prefabFile >> line;

			for (int i = 0; i < line.length(); i++)
			{
				// block
				if (line[i] == '1')
					prefab->Entities[lineIndex][i] = new Entity("blockhere");
			}

			lineIndex++;
		}
		m_level.push_back(prefab);
	}
}

Prefab* PrefabManager::FetchPrefab(const int p_seed)
{
	return nullptr;
}
