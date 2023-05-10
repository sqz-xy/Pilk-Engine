#pragma once

#include "PilkEngineCommon.h"
#include "EntityManager.h"

#include "Entity.h"
#include "SystemManager.h"

EntityManager::EntityManager()
{
}

/// Original Author: Thomas Beet
/// <summary>
/// Clears the entities
/// </summary>
EntityManager::~EntityManager()
{
	Clear();
}

/// Original Author: Thomas Beet
/// <summary>
/// Adds an entity to the manager list
/// </summary>
bool EntityManager::AddEntity(Entity* p_entity)
{
	// Cant be null
	if (p_entity == nullptr)
		return false;

	// If there are no entities, no need for an ID check
	if (m_entities.empty())
	{
		m_entities.push_back(p_entity);
		return true;
	}

	// Entites cant have the same IDs
	for (int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i]->GetID() == p_entity->GetID())
		{
			std::cout << "Can't have multiple entities with the same ID" << std::endl;
			return false;
		}

		m_entities.push_back(p_entity);
		return true;
	}
}

/// Original Author: Thomas Beet
/// <summary>
/// Removes an entity based on its ID
/// </summary>
bool EntityManager::RemoveEntity(const int p_id)
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i]->GetID() == p_id)
		{
			delete m_entities[i]; 
			m_entities.erase(m_entities.begin() + i);	
			return true;
		}
	}
	return false;
}

/// Original Author: Piotr Moskala
/// <summary>
/// Validates every tracked Entity against each system in the System Manager
/// </summary>
/// <param name="p_systemManager"> The System Manager that handles the validation against its systems </param>
void EntityManager::ValidateEntities(SystemManager* p_systemManager)
{
	int size = m_entities.size();
	for (int i = 0; i < size; i++)
	{
		p_systemManager->ValidateEntity(m_entities[i]);
	}
}

/// Original Author: Piotr Moskala
/// <summary>
/// Deletes all tracked Entities from memory and clears pointers.
/// </summary>
void EntityManager::Clear()
{
	int size = m_entities.size();
	for (int i = 0; i < size; i++)
	{
		delete m_entities[i];
	}
	m_entities.clear();
}

/// Original Author: Thomas Beet
/// <summary>
/// Finds an entity based on ID
/// </summary>
/// <param name="p_name">Entity to find</param>
/// <returns>Entity or nullptr</returns>
Entity* EntityManager::FindEntity(const int p_id)
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i]->GetID() == p_id)
		{
			return m_entities[i];
		}
	}
	return nullptr;
}

/// Original Author: Thomas Beet
/// <summary>
/// Finds an entity based on name
/// </summary>
/// <param name="p_name">Entity to find</param>
/// <returns>Entity or nullptr</returns>
Entity* EntityManager::FindEntity(const std::string& p_name)
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i]->GetName() == p_name)
		{
			return m_entities[i];
		}
	}
	return nullptr;
}

/// Original Author: Thomas Beet
/// <summary>
/// Finds entities with the same names
/// </summary>
std::vector<Entity*> EntityManager::FindEntities(const std::string& p_name)
{
	std::vector<Entity*> entities;
	for (int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i]->GetName() == p_name)
		{
			entities.push_back(m_entities[i]);
		}
	}
	return entities;
}

/// Original Author: Thomas Beet
/// <summary>
/// Gets entity count
/// </summary>
/// <returns>EntityCount</returns>
int EntityManager::EntityCount()
{
	return m_entities.size();
}
