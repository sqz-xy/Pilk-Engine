#pragma once

class SystemManager;
class EntityManager;
class Entity;

enum CollisionType
{
	AABB_AABB_COLLISION_LEFT,
	AABB_AABB_COLLISION_RIGHT,
	AABB_AABB_COLLISION_TOP,
	AABB_AABB_COLLISION_BOTTOM,
	AABB_POINT,
	AABB_SPHERE_POINT
};

struct Collision
{
	Entity* m_entity1;
	Entity* m_entity2;
	CollisionType m_collisionType;
};

class CollisionManager
{
public:
	explicit CollisionManager(SystemManager* p_systemManager, EntityManager* p_entityManager)
	{
		m_systemManager = p_systemManager;
		m_entityManager = p_entityManager;
	}
	~CollisionManager();
	void ClearManifold();
	void RegisterCollision(Entity* p_entity1, Entity* p_entity2, const CollisionType p_collisionType);
	virtual void ProcessCollisions() = 0;

protected:
	std::vector<Collision*> m_collisionManifold;
	SystemManager* m_systemManager;
	EntityManager* m_entityManager;
};