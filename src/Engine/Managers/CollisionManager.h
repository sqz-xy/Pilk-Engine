#include "../PilkEngineCommon.h"

#include "../Objects/Entity.h"

enum CollisionType
{
	AABB_AABB_COLLISION
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
	void ClearManifold();
	void RegisterCollision(Entity* p_entity1, Entity* p_entity2, const CollisionType p_collisionType);
	virtual void ProcessCollisions() = 0;

protected:
	explicit CollisionManager() {}
	~CollisionManager();

	std::vector<Collision*> m_collisionManifold;

};