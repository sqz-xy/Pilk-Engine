#pragma once

#include "../PilkEngineCommon.h"
#include "../Objects/Entity.h"

class EntityManager final
{
public:
	explicit EntityManager();
	bool AddEntity(Entity* p_entity);
	bool RemoveEntity(const std::string& p_name);

private:
	std::vector<Entity*> m_entities;
};