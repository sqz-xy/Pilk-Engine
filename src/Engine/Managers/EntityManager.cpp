#include "EntityManager.h"

EntityManager::EntityManager()
{
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
