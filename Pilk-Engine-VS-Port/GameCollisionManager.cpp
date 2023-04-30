#include "PilkEngineCommon.h"

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
		}
	}

	void RespondAABBAABBLeftRight(Collision* p_col)
	{
		Entity* e1 = p_col->m_entity1;
		Entity* e2 = p_col->m_entity2;

		ComponentTransform* pos1 = e1->GetComponent<ComponentTransform>();
		ComponentTransform* pos2 = e2->GetComponent<ComponentTransform>();

		ComponentCollisionAABB* col1 = e1->GetComponent<ComponentCollisionAABB>();
		ComponentCollisionAABB* col2 = e2->GetComponent<ComponentCollisionAABB>();

		vec3 new_pos = vec3(pos2->m_translation.x, pos1->m_translation.y, pos1->m_translation.z);

		if (p_col->m_collisionType == AABB_AABB_COLLISION_RIGHT)
		{
			new_pos.x += ((col1->GetWidth() / 2) + (col2->GetWidth() / 2));
		}
		else if (p_col->m_collisionType == AABB_AABB_COLLISION_LEFT)
		{
			new_pos.x -= ((col1->GetWidth()/2) + (col2->GetWidth()/2));
		}

		pos1->UpdateTranslation(new_pos);

	}

	void RespondAABBAABBUpDown(Collision* p_col)
	{
		Entity* e1 = p_col->m_entity1;
		Entity* e2 = p_col->m_entity2;

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

	void RespondAABBPoint(Collision* p_col)
	{
		ComponentPhysics* phys1 = p_col->m_entity1->GetComponent<ComponentPhysics>();
		phys1->SetGravity(vec3(0, 0, 0));
		phys1->SetCurrentGravity(vec3(0, 0, 0));

		ComponentProperties* prop = p_col->m_entity1->GetComponent<ComponentProperties>();
		prop->m_hasJumped = false;
	}
};