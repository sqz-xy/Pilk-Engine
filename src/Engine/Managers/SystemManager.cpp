#include "SystemManager.h"

SystemManager::SystemManager()
{

}

/// Original Author: Piotr Moskala
/// <summary>
/// Deletes systems from memory
/// </summary>
SystemManager::~SystemManager()
{
	Clear();
}

/// Original Author: Piotr Moskala
/// <summary>
/// Adds a System to the System Manager.
/// </summary>
/// <param name="p_system"> System to add </param>
void SystemManager::AddSystem(System* p_system)
{
	systems.push_back(p_system);
}

/// Original Author: Piotr Moskala
/// <summary>
/// Deletes all tracked Systems from memory and clears pointers.
/// </summary>
void SystemManager::Clear()
{
	int size = systems.size();
	for (int i = 0; i < size; i++)
	{
		delete systems[i];
	}
	systems.clear();
}

/// Original Author: Piotr Moskala
/// <summary>
/// Validates entity against every system and its required components
/// </summary>
/// <param name="p_entity"> Entity to validate </param>
void SystemManager::ValidateEntity(Entity* p_entity)
{
	int size = systems.size();
	for (int i = 0; i < size; i++)
	{
		systems[i]->ValidateEntity(p_entity);
	}
}
