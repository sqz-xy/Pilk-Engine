#include "PilkEngineCommon.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
	Clear();
}

bool EntityManager::AddEntity(Entity* p_entity)
{
	if (m_entities.empty())
	{
		m_entities.push_back(p_entity);
		return true;
	}

	for (int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i]->GetName() == p_entity->GetName())
		{
			std::cout << "Can't have multiple entities with the same name" << std::endl;
			return false;
		}

		m_entities.push_back(p_entity);
		return true;
	}
}

bool EntityManager::RemoveEntity(const std::string& p_name)
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i]->GetName() == p_name)
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
