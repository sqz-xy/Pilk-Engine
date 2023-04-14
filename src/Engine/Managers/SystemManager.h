#pragma once

#include <vector>

#include "../Systems/System.h"
#include "../Objects/Entity.h"

class SystemManager
{
public:
	SystemManager();
	~SystemManager();

	void AddSystem(System* p_system);
	void Clear();

	void ValidateEntity(Entity* p_entity);

private:
	std::vector<System*> systems;
};