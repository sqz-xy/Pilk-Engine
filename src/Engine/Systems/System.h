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
	~System() {}

	/// Original Author: Piotr Moskala
	/// <summary>
	/// Executes the specified behaviour of the system
	/// </summary>
	virtual void Execute(const float p_deltaTime) {}

	/// Original Author: Piotr Moskala
	/// <summary>
	/// Override this method to define valid components and pass them into the other overloaded ValidateEntity()
	/// </summary>
	/// <param name="p_entity"> The entity to validate </param>
	virtual void ValidateEntity(Entity* p_entity) {}

	/// Original Author: Piotr Moskala
	/// <summary>
	/// Checks if an entity has the required components.
	/// If it does, and the entity is not in the list
	/// </summary>
	/// <param name="p_entity"></param>
	void ValidateEntity(Entity* p_entity, bool p_validComponents)
	{
		bool systemContainsEntity = ContainsEntity(p_entity);

		if (p_validComponents && !systemContainsEntity)
		{
			validEntities.push_back(p_entity);
		}
		else if (systemContainsEntity)
		{
			RemoveEntity(p_entity);
		}
	}

	/// Original Author: Piotr Moskala
	/// <summary>
	/// Checks if the system contains a specific entity
	/// </summary>
	/// <param name="p_entity"> The entity to check for </param>
	/// <returns> 'true' if contains, 'false' if doesn't </returns>
	bool ContainsEntity(Entity* p_entity)
	{
		// Empty
		if (validEntities.empty())
			return false;

		// Entity is found
		if (std::find(validEntities.begin(), validEntities.end(), p_entity) != validEntities.end())
			return true;

		// Entity is not found
		return false;
	}

	// Original Author: Piotr Moskala
	/// <summary>
	/// Removes a specific entity from the valid entities list of this system
	/// </summary>
	/// <param name="p_entity"> The entity to remove </param>
	void RemoveEntity(Entity* p_entity)
	{
		validEntities.erase(std::remove(validEntities.begin(), validEntities.end(), p_entity), validEntities.end());
	}

protected:
	std::vector<Entity*> validEntities;
};


class SystemPhysics : public System
{
public:
	SystemPhysics() {};

	/// Original Author: Piotr Moskala
	/// <summary>
	/// Orchestrates the physics of the entity using its Transform and X Components 
	/// </summary>
	virtual void Execute(const float p_deltaTime) override
	{
		for each (Entity* entity in validEntities)
		{
			ComponentTransform* componentTransform = entity->GetComponent<ComponentTransform>();
			if (componentTransform == nullptr) return;

			// TODO: Calculate new transformation with component Physics/Velocity/etc.
			
			/*glm::vec3 newTranslation = componentTransform->m_translation + glm::vec3(0.0f, 0.0f, 0.2f * p_deltaTime);
			componentTransform->UpdateTranslation(newTranslation); */ // Remove these when TODO complete
		}
	}

	/// Original Author: Piotr Moskala
	/// <summary>
	/// Checks for valid SystemPhysics components, then validates the entity against them
	/// </summary>
	/// <param name="p_entity"> Entity to validate </param>
	virtual void ValidateEntity(Entity* p_entity) override
	{
		// Specify valid components separated by "&&" here: 
		bool requiredComponents =
			p_entity->GetComponent<ComponentTransform>() != nullptr;


		System::ValidateEntity(p_entity, requiredComponents);
	}
};

/// <summary>
/// Render system by Thomas Beet
/// </summary>
class SystemRender : public System
{
public:
	SystemRender() {}

	virtual void Execute(const float p_deltaTime) override
	{
		for each (Entity* entity in validEntities)
		{
			ComponentTransform* transform = entity->GetComponent<ComponentTransform>();
			ComponentGeometry* geometry = entity->GetComponent<ComponentGeometry>();
			ComponentShader* shader = entity->GetComponent<ComponentShader>();

			shader->UseShader(&transform->m_transform);
			geometry->Draw(shader->m_shaderProgramID);
		}
	}

	virtual void ValidateEntity(Entity* p_entity) override
	{
		bool requiredComponents = p_entity->GetComponent<ComponentTransform>() != nullptr &&
								  p_entity->GetComponent<ComponentGeometry>() != nullptr &&
								  p_entity->GetComponent<ComponentShader>() != nullptr;

		System::ValidateEntity(p_entity, requiredComponents);
	}
};