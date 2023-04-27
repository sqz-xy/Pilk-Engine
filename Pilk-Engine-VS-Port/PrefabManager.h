#pragma once

#include "PilkEngineCommon.h"

// Prefabs are 6x6

struct Prefab 
{
	// Prefab entities
	Entity* Entities[6][6];

	// These entities will be copied to fill the prefab
	Entity* WallEntity;
	std::vector<Entity*> EnemyEntities;

	int Width;
	int Height;
};

// By Thomas Beet
class PrefabManager final
{
public:
	explicit PrefabManager();
	void LoadPrefabs(const std::string& p_prefabPath, const std::string& p_entityScriptPath);
	Prefab* FetchPrefab(const int p_seed);

private:
	std::vector<Prefab> m_prefabs;
};
