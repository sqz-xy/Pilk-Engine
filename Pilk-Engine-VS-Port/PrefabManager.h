#pragma once

class Entity;
class SystemManager;
class EntityManager;

// Prefabs are 6x6

const int PREFAB_SIZE = 6;

/// Original Author: Thomas Beet
/// <summary>
/// Holds all entities within a 6x6 prefab
/// </summary>
struct Prefab 
{
	// Prefab entities
	Entity* Entities[PREFAB_SIZE][PREFAB_SIZE];
	Entity* BackWall;
	Entity* Ceiling;
	Entity* Floor;
};

/// Original Author: Thomas Beet
/// <summary>
/// Holds a list of prefabs to make a level
/// </summary>
struct Level
{
	std::vector<Prefab*> level;
	float width;
};

/// Original Author: Thomas Beet
/// <summary>
/// Prefab manager declarations
/// </summary>
class PrefabManager final
{
public:
	explicit PrefabManager();
	void LoadPrefabs(const std::string& p_prefabPath, const std::string& p_entityScriptPath);
	Level RegisterLevel(EntityManager& p_entityManager, SystemManager& p_systemManager);

private:
	Level m_level;
};
