#pragma once

#include "PilkEngineCommon.h"

#include "CollisionManager.h"

/// Original Author: Thomas Beet
/// <summary>
/// Clearing the collision manifold
/// </summary>
void CollisionManager::ClearManifold()
{
	m_collisionManifold.clear();
}

/// Original Author: Thomas Beet
/// <summary>
/// If a collision happens the entities are passed in here and a collision object is made, added to the manifold.
/// </summary>
void CollisionManager::RegisterCollision(Entity* p_entity1, Entity* p_entity2, const CollisionType p_collisionType)
{
	for (Collision* col : m_collisionManifold)
	{
		// Makes sure the collision doesn't already exist
		if (col->m_entity1 == p_entity1 && col->m_entity2 == p_entity2 && col->m_collisionType == p_collisionType)
			return;
	}

	// Create collision
	Collision* col = new Collision();
	col->m_entity1 = p_entity1;
	col->m_entity2 = p_entity2;
	col->m_collisionType = p_collisionType;

	m_collisionManifold.push_back(col);
}

/// Original Author: Thomas Beet
/// <summary>
/// Clears the collision manager on close
/// </summary>
CollisionManager::~CollisionManager()
{
	m_collisionManifold.clear();
}
