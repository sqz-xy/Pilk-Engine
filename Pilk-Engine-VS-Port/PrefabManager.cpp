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

		while (!prefabFile.eof())
		{
			std::string line;
			prefabFile >> line;
			std::cout << line << std::endl;
		}
	}
}

Prefab* PrefabManager::FetchPrefab(const int p_seed)
{
	return nullptr;
}
