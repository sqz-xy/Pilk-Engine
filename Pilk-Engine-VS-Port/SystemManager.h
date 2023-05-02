#pragma once

#include "PilkEngineCommon.h"

#include "System.h"
#include "Entity.h"

class SystemManager final
{
public:
	SystemManager();
	~SystemManager();

	void AddSystem(System* p_system);
	void ExecuteSystems(const float p_deltaTime);
	void Clear();

	void ValidateEntity(Entity* p_entity);
	void RemoveEntity(Entity* p_entity);

private:
	std::vector<System*> systems;
};