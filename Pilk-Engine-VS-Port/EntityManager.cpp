#pragma once

#include "PilkEngineCommon.h"
#include "EntityManager.h"

#include "Entity.h"
#include "SystemManager.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
	Clear();
}

bool EntityManager::AddEntity(Entity* p_entity)
{
	if (p_entity == nullptr)
		return false;

	if (m_entities.empty())
	{
		m_entities.push_back(p_entity);
		return true;
	}

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

bool EntityManager::RemoveEntity(const int p_id)
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i]->GetID() == p_id)
		{
			delete m_entities[i]; // Mem leak
			m_entities.erase(m_entities.begin() + i);	
			return true;
		}

		return false;
	}
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
/// Gets entity count
/// </summary>
/// <returns>EntityCount</returns>
int EntityManager::EntityCount()
{
	return m_entities.size();
}
