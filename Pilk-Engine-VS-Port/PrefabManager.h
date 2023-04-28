#pragma once

#include "PilkEngineCommon.h"

// Prefabs are 6x6

const int PREFAB_SIZE = 6;

struct Prefab 
{
	// Prefab entities
	Entity* Entities[PREFAB_SIZE][PREFAB_SIZE];

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
	std::vector<Prefab*> RegisterLevel(EntityManager& p_entityManager, SystemManager& p_systemManager);

private:
	std::vector<Prefab*> m_level;
};
