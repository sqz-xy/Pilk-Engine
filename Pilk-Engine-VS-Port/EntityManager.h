#pragma once

class SystemManager;
class Entity;

class EntityManager final
{
public:
	explicit EntityManager();
	~EntityManager();

	bool AddEntity(Entity* p_entity);
	bool RemoveEntity(const int p_id);
	void ValidateEntities(SystemManager* p_systemManager);
	void Clear();

	Entity* FindEntity(const int p_id);
	Entity* FindEntity(const std::string& p_name);
	std::vector<Entity*> FindEntities(const std::string& p_name);

	int EntityCount();

	std::vector<Entity*> m_entities;
private:
};