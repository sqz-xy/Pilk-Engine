#pragma once


#include "PilkEngineCommon.h"

#include "CollisionManager.h"

class GameCollisionManager : public CollisionManager
{
public:
	void ProcessCollisions() override
	{
		for (Collision* col : m_collisionManifold)
		{
			if (col->m_collisionType == AABB_AABB_COLLISION_RIGHT || col->m_collisionType == AABB_AABB_COLLISION_LEFT)
			{
				RespondAABBAABBLeftRight(col);
			}
			else if (col->m_collisionType == AABB_AABB_COLLISION_TOP || col->m_collisionType == AABB_AABB_COLLISION_BOTTOM)
			{
				RespondAABBAABBUpDown(col);
			}
			else if (col->m_collisionType == AABB_POINT)
			{
				RespondAABBPoint(col);
			}
			else if (col->m_collisionType == AABB_SPHERE_POINT)
			{
				RespondPointSphere(col);
			}
		}
	}

	void RespondAABBAABBLeftRight(Collision* p_col)
	{

		Entity* e1 = p_col->m_entity1;
		Entity* e2 = p_col->m_entity2;

		if ((e1->GetName() == "Bullet" && e2->GetName() == "Block") || (e2->GetName() == "Bullet" && e1->GetName() == "Block"))
		{
			BulletWall(e1, e2);
		}

		ComponentTransform* pos1 = e1->GetComponent<ComponentTransform>();
		ComponentTransform* pos2 = e2->GetComponent<ComponentTransform>();

		ComponentCollisionAABB* col1 = e1->GetComponent<ComponentCollisionAABB>();
		ComponentCollisionAABB* col2 = e2->GetComponent<ComponentCollisionAABB>();

		vec3 new_pos = vec3(pos2->m_translation.x, pos1->m_translation.y, pos1->m_translation.z);

		if (p_col->m_collisionType == AABB_AABB_COLLISION_RIGHT)
		{
			new_pos.x += ((col1->GetWidth() / 2) + (col2->GetWidth() / 2));

			if (p_col->m_entity1->GetName() == "GroundEnemy" || p_col->m_entity1->GetName() == "FlyingEnemy")
			{
				ComponentPhysics* phys = p_col->m_entity1->GetComponent<ComponentPhysics>();
				phys->SetVelX(-phys->GetVelocity().x);
			}
		}
		else if (p_col->m_collisionType == AABB_AABB_COLLISION_LEFT)
		{
			new_pos.x -= ((col1->GetWidth()/2) + (col2->GetWidth()/2));
			if (p_col->m_entity1->GetName() == "GroundEnemy" || p_col->m_entity1->GetName() == "FlyingEnemy")
			{
				ComponentPhysics* phys = p_col->m_entity1->GetComponent<ComponentPhysics>();
				phys->SetVelX(-phys->GetVelocity().x);
			}
		}

		pos1->UpdateTranslation(new_pos);

	}

	void RespondAABBAABBUpDown(Collision* p_col)
	{
		Entity* e1 = p_col->m_entity1;
		Entity* e2 = p_col->m_entity2;

		if ((e1->GetName() == "Bullet" && e2->GetName() == "Block") || (e2->GetName() == "Bullet" && e1->GetName() == "Block"))
		{
			BulletWall(e1, e2);
		}

		ComponentTransform* pos1 = e1->GetComponent<ComponentTransform>();
		ComponentTransform* pos2 = e2->GetComponent<ComponentTransform>();

		ComponentCollisionAABB* col1 = e1->GetComponent<ComponentCollisionAABB>();
		ComponentCollisionAABB* col2 = e2->GetComponent<ComponentCollisionAABB>();

		vec3 new_pos = vec3(pos1->m_translation.x, pos2->m_translation.y, pos1->m_translation.z);

		if (p_col->m_collisionType == AABB_AABB_COLLISION_TOP)
		{
			new_pos.y += ((col1->GetHeight() / 2) + (col2->GetHeight() / 2));
		}
		else
		{
			new_pos.y -= ((col1->GetHeight() / 2) + (col2->GetHeight() / 2));
		}

		pos1->UpdateTranslation(new_pos);
	}

	void BulletWall(Entity* p_e1, Entity* p_e2)
	{
		if (p_e1->GetName() == "Bullet")
		{
			// derlete e1
		}
		else
		{
			// delte 2
		}
	}

	void RespondAABBPoint(Collision* p_col)
	{
		ComponentPhysics* phys1 = p_col->m_entity1->GetComponent<ComponentPhysics>();
		phys1->SetGravity(vec3(0, 0, 0));
		phys1->SetCurrentGravity(vec3(0, 0, 0));
		phys1->SetVelY(0);

		ComponentProperties* prop = p_col->m_entity1->GetComponent<ComponentProperties>();
		prop->m_hasJumped = false;
	}

	void RespondPointSphere(Collision* p_col)
	{
		ComponentTransform* trans1 = p_col->m_entity1->GetComponent<ComponentTransform>();

		ComponentPhysics* phys2 = p_col->m_entity2->GetComponent<ComponentPhysics>();
		ComponentTransform* trans2 = p_col->m_entity2->GetComponent<ComponentTransform>();

		vec3 pos1 = trans1->m_translation;
		vec3 pos2 = trans2->m_translation;
		vec3 distance = pos2 - pos1;
		distance = glm::normalize(distance) * 2.0f;

		phys2->SetVelVector(-distance);
	}
};