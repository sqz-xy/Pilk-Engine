#include "PilkEngineCommon.h"

class GameCollisionManager : public CollisionManager
{
public:
	void ProcessCollisions() override
	{
		for (Collision* col : m_collisionManifold)
		{
			if (col->m_collisionType == AABB_AABB_COLLISION_LEFT)
			{
				RespondAABBAABBLeft(col);
			}
			else if (col->m_collisionType == AABB_AABB_COLLISION_RIGHT)
			{
				RespondAABBAABBRight(col);
			}
			else if (col->m_collisionType == AABB_AABB_COLLISION_TOP)
			{
				RespondAABBAABBUp(col);
			}
			else if (col->m_collisionType == AABB_AABB_COLLISION_BOTTOM)
			{
				RespondAABBAABBDown(col);
			}
		}
	}

	void RespondAABBAABBLeft(Collision* p_col)
	{

	}
	void RespondAABBAABBRight(Collision* p_col)
	{

	}
	void RespondAABBAABBUp(Collision* p_col)
	{
		Entity* e1 = p_col->m_entity1;
		Entity* e2 = p_col->m_entity2;

		ComponentTransform* pos1 = e1->GetComponent<ComponentTransform>();
		ComponentTransform* pos2 = e2->GetComponent<ComponentTransform>();

		ComponentCollisionAABB* col2 = e2->GetComponent<ComponentCollisionAABB>();

		vec3 new_pos = vec3(pos1->m_translation.x, pos2->m_translation.y, pos1->m_translation.z);
		new_pos.y += col2->GetHeight();

		pos1->UpdateTranslation(new_pos);
	}
	void RespondAABBAABBDown(Collision* p_col)
	{

	}
};