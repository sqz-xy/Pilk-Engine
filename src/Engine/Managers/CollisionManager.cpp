#include "CollisionManager.h"

void CollisionManager::ClearManifold()
{
	m_collisionManifold.clear();
}

void CollisionManager::RegisterCollision(Entity* p_entity1, Entity* p_entity2, const CollisionType p_collisionType)
{
	for each (Collision* col in m_collisionManifold)
	{
		if (col->m_entity1 == p_entity1 && col->m_entity2 == p_entity2)
			return;
	}

	Collision* col = new Collision();
	col->m_entity1 = p_entity1;
	col->m_entity2 = p_entity2;
	col->m_collisionType = p_collisionType;
}

CollisionManager::~CollisionManager()
{
	m_collisionManifold.clear();
}
