#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <vector>
#include <algorithm>

#include "../Objects/Entity.h"

class System
{
public:
	System() {};
	~System() {};

	virtual void OnAction(const float deltaTime) = 0;

	virtual void UpdateEntity(Entity* entity) = 0;

	bool ContainsEntity(Entity* entity)
	{
		// Empty
		if (!validEntities.empty())
			return false;

		// Entity is found
		if (std::find(validEntities.begin(), validEntities.end(), entity) != validEntities.end())
			return true;

		// Entity is not found
		return false;
	}

protected:
	std::vector<Entity*> validEntities;
};


class SystemPhysics : System
{
public:
	SystemPhysics() {};

	virtual void OnAction(const float deltaTime) override
	{
		for each (Entity* entity in validEntities)
		{
			ComponentTransform* componentTransform = entity->GetComponent<ComponentTransform>();
			glm::vec3 newTranslation = componentTransform->m_translation + glm::vec3(1.0f * deltaTime, 0.0f, 0.0f);
			componentTransform->UpdateTranslation(newTranslation);
		}
	}

	virtual void UpdateEntity(Entity* entity) override
	{
		if (entity->GetComponent<ComponentTransform>() == nullptr) return;

		validEntities.push_back(entity);
	}
};