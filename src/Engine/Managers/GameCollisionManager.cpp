#include "../PilkEngineCommon.h"
#include "CollisionManager.h"

class GameCollisionManager : public CollisionManager
{
public:
	void ProcessCollisions() override
	{
		for each (Collision * col in m_collisionManifold)
		{

		}
	}
};