#pragma once

#include "Entity.h"
#include "SystemManager.h"
#include "EntityManager.h"

// Prefabs are 6x6

const int PREFAB_SIZE = 6;


struct Prefab 
{
	// Prefab entities
	Entity* Entities[PREFAB_SIZE][PREFAB_SIZE];
	Entity* BackWall;
	Entity* Ceiling;
	Entity* Floor;
};

struct Level
{
	std::vector<Prefab*> level;
	float width;
};

// By Thomas Beet
class PrefabManager final
{
public:
	explicit PrefabManager();
	void LoadPrefabs(const std::string& p_prefabPath, const std::string& p_entityScriptPath);
	Level RegisterLevel(EntityManager& p_entityManager, SystemManager& p_systemManager);

private:
	Level m_level;
};
