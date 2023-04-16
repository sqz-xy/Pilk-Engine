#pragma once

#include "../PilkEngineCommon.h"
#include "../Objects/Entity.h"
#include "../Managers/SystemManager.h"

class EntityManager final
{
public:
	explicit EntityManager();
	~EntityManager();

	bool AddEntity(Entity* p_entity);
	bool RemoveEntity(const std::string& p_name);
	void ValidateEntities(SystemManager* p_systemManager);
	void Clear();

private:
	std::vector<Entity*> m_entities;
};