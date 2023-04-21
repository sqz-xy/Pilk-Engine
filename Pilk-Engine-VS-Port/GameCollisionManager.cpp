#include "PilkEngineCommon.h"

class GameCollisionManager : public CollisionManager
{
public:
	void ProcessCollisions() override
	{
		for (Collision* col : m_collisionManifold)
		{
			
		}
	}
};