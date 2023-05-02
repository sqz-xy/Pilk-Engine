#pragma once

#include "PilkEngineCommon.h"

#include "CollisionManager.h"
#include "Entity.h"

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
	/// Logic By: Matthew Liney
	/// <summary>
	/// Orchestrates the physics of the entity using its Transform and X Components 
	/// </summary>
	virtual void Execute(const float p_deltaTime) override
	{
		for (Entity* entity : validEntities)
		{
			ComponentTransform* componentTransform = entity->GetComponent<ComponentTransform>();
			if (componentTransform == nullptr) return;

			ComponentPhysics* componentPhysics = entity->GetComponent<ComponentPhysics>();
			if (componentPhysics == nullptr) return;

			vec3 vel = componentPhysics->GetVelocity() * p_deltaTime;
			vec3 grav = componentPhysics->GetGravity() * p_deltaTime;
			vec3 temp = componentPhysics->GetCurrentGravity();
			vec3 newGrav = componentPhysics->GetCurrentGravity() += grav;

			componentPhysics->SetCurrentGravity(newGrav);


			vec3 pos = componentTransform->m_translation;
			vec3 newPos = (pos + vel) + newGrav;
			//vec3 newPos = (pos + vel);

			componentTransform->UpdateTranslation(newPos);
		}
	}

	/// Original Author: Piotr Moskala
	/// Edited By: Matthew Liney
	/// <summary>
	/// Checks for valid SystemPhysics components, then validates the entity against them
	/// </summary>
	/// <param name="p_entity"> Entity to validate </param>
	virtual void ValidateEntity(Entity* p_entity) override
	{
		// Specify valid components separated by "&&" here: 
		bool requiredComponents =
			p_entity->GetComponent<ComponentTransform>() != nullptr &&
			p_entity->GetComponent<ComponentPhysics>() != nullptr;


		System::ValidateEntity(p_entity, requiredComponents);
	}
};

/// <summary>
/// Render system by Thomas Beet
/// </summary>
class SystemRender : public System
{
public:
	SystemRender(Camera* p_camera) 
	{
		m_camera = p_camera;
	}

	virtual void Execute(const float p_deltaTime) override
	{
		for (Entity* entity : validEntities)
		{
			ComponentTransform* transform = entity->GetComponent<ComponentTransform>();
			ComponentGeometry* geometry = entity->GetComponent<ComponentGeometry>();
			ComponentShader* shader = entity->GetComponent<ComponentShader>();

			shader->UseShader(&transform->m_transform, &m_camera->m_view, &m_camera->m_projection, transform);
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
private:
	Camera* m_camera;
};



// class by matthew liney
class SystemCollisionAABBAABB : public System
{
public:

	// cm stands for collision manager, by the way.
	SystemCollisionAABBAABB(CollisionManager* p_cm)
	{
		m_cm = p_cm;
	}

	//AHHHHH we need rto make it so it checks entities with phys against all others
	// because we dont need to move ahything without phys component
	// also will make more effocoemt amd npot break

	virtual void Execute(const float p_deltaTime) override
	{
		for (Entity* entity1 : validEntities)
		{
			ComponentPhysics* phys = entity1->GetComponent<ComponentPhysics>();

			if (phys != nullptr)
			{
				for (Entity* entity2 : validEntities)
				{
					if (entity1 != entity2)
					{
						CollisionCheck(entity1, entity2);
					}
				}
			}
		}


	}

	virtual void CollisionCheck(Entity* p_entity_1, Entity* p_entity_2)
	{
		ComponentTransform* e1_transform = p_entity_1->GetComponent<ComponentTransform>();
		vec3 e1_pos = e1_transform->m_translation;
		ComponentCollisionAABB* e1_collision = p_entity_1->GetComponent<ComponentCollisionAABB>();
		float e1_height = e1_collision->GetHeight();
		float e1_width = e1_collision->GetWidth();
		float e1_depth = e1_collision->GetDepth();

		ComponentTransform* e2_transform = p_entity_2->GetComponent<ComponentTransform>();
		vec3 e2_pos = e2_transform->m_translation;
		ComponentCollisionAABB* e2_collision = p_entity_2->GetComponent<ComponentCollisionAABB>();
		float e2_height = e2_collision->GetHeight();
		float e2_width = e2_collision->GetWidth();
		float e2_depth = e2_collision->GetDepth();


		// a min and max values
		float a_x_min = e1_pos.x - (e1_collision->GetWidth() / 2);
		float a_x_max = e1_pos.x + (e1_collision->GetWidth() / 2);

		float a_y_min = e1_pos.y - (e1_collision->GetHeight() / 2);
		float a_y_max = e1_pos.y + (e1_collision->GetHeight() / 2);

		// b min and max values
		float b_x_min = e2_pos.x - (e2_collision->GetWidth() / 2);
		float b_x_max = e2_pos.x + (e2_collision->GetWidth() / 2);

		float b_y_min = e2_pos.y - (e2_collision->GetHeight() / 2);
		float b_y_max = e2_pos.y + (e2_collision->GetHeight() / 2);


		if (a_x_max > b_x_min && a_x_min < b_x_max &&
			a_y_max > b_y_min && a_y_min < b_y_max) // add z to this when u can be bothered
		{
			vec3 distance = e1_pos - e2_pos;
			float x_extent_1 = e1_width / 2;
			float x_extent_2 = e2_width / 2;
			float x_overlap = (x_extent_1 - x_extent_2) - glm::abs(distance.x);

			float y_extent_1 = e1_height / 2;
			float y_extent_2 = e2_height / 2; // do this to z as well
			float y_overlap = (y_extent_1 - y_extent_2) - glm::abs(distance.y);

			if (p_entity_2->GetName() == "Ceiling")
			{
				m_cm->RegisterCollision(p_entity_1, p_entity_2, AABB_AABB_COLLISION_BOTTOM);
				return;
			}

			if (y_overlap > x_overlap)
			{
				// to do:
				// there will need to be collision types for all 4 sides
				// 6 if we want depth-wise
				// debug and figure out which side is represented...

				if (distance.x > 0)
				{
					m_cm->RegisterCollision(p_entity_1, p_entity_2, AABB_AABB_COLLISION_RIGHT);
				}
				else
				{
					m_cm->RegisterCollision(p_entity_1, p_entity_2, AABB_AABB_COLLISION_LEFT);
				}
			}
			else
			{
				if (distance.y > 0)
				{
					m_cm->RegisterCollision(p_entity_1, p_entity_2, AABB_AABB_COLLISION_TOP);
				}
				else
				{
					m_cm->RegisterCollision(p_entity_1, p_entity_2, AABB_AABB_COLLISION_BOTTOM);
				}
			}
		}
	}

	virtual void ValidateEntity(Entity* p_entity) override
	{
		bool requiredComponents = p_entity->GetComponent<ComponentCollisionAABB>() != nullptr &&
			p_entity->GetComponent<ComponentTransform>() != nullptr;

		System::ValidateEntity(p_entity, requiredComponents);
	}

private:
	CollisionManager* m_cm;
};

// class by matthew liney
class SystemCollisionAABBPoint : public System
{
public:

	// cm stands for collision manager, by the way.
	SystemCollisionAABBPoint(CollisionManager* p_cm)
	{
		m_cm = p_cm;
	}

	virtual void Execute(const float p_deltaTime) override
	{
		for (Entity* entity1 : validEntities)
		{
			ComponentPhysics* phys = entity1->GetComponent<ComponentPhysics>();

			if (phys != nullptr)
			{
				for (Entity* entity2 : validEntities)
				{

					if (entity1 != entity2)
					{
						CollisionCheck(entity1, entity2);
					}
				}
			}
		}


	}

	virtual void CollisionCheck(Entity* p_entity_1, Entity* p_entity_2)
	{
		ComponentTransform* trans1 = p_entity_1->GetComponent<ComponentTransform>();
		ComponentCollisionPoint* point1 = p_entity_1->GetComponent<ComponentCollisionPoint>();
		vec3 point = trans1->m_translation + point1->GetPoint();

		ComponentTransform* trans2 = p_entity_2->GetComponent<ComponentTransform>();
		ComponentCollisionAABB* aabb1 = p_entity_2->GetComponent<ComponentCollisionAABB>();
		vec3 pos2 = trans2->m_translation;


		vec2 top_left = vec2(pos2.x - (aabb1->GetWidth() / 2), pos2.y + (aabb1->GetHeight() / 2));
		vec2 bottom_right = vec2(pos2.x + (aabb1->GetWidth() / 2), pos2.y - (aabb1->GetHeight() / 2));

		if (point.x > top_left.x && point.x < bottom_right.x)
		{
			if (point.y < top_left.y && point.y > bottom_right.y)
			{
				m_cm->RegisterCollision(p_entity_1, p_entity_2, AABB_POINT);
			}
		}
		else if(p_entity_1->GetName() != "FlyingEnemy")
		{
			ComponentPhysics* phys = p_entity_1->GetComponent<ComponentPhysics>();
			phys->SetGravity(vec3(0, -0.3f, 0));
			ComponentProperties* prop = p_entity_1->GetComponent<ComponentProperties>();
			prop->m_hasJumped = true;
		}
	}

	virtual void ValidateEntity(Entity* p_entity) override
	{
		bool requiredComponents = (p_entity->GetComponent<ComponentCollisionPoint>() != nullptr &&
			p_entity->GetComponent<ComponentTransform>() != nullptr) || (p_entity->GetComponent<ComponentCollisionAABB>() != nullptr &&
				p_entity->GetComponent<ComponentTransform>() != nullptr);

		System::ValidateEntity(p_entity, requiredComponents);
	}

private:
	CollisionManager* m_cm;
};

// class by matthew liney
class SystemCollisionSpherePoint : public System
{
public:

	// cm stands for collision manager, by the way.
	SystemCollisionSpherePoint(CollisionManager* p_cm)
	{
		m_cm = p_cm;
	}

	virtual void Execute(const float p_deltaTime) override
	{
		for (Entity* entity1 : validEntities)
		{
			ComponentPhysics* phys1 = entity1->GetComponent<ComponentPhysics>();
			ComponentCollisionPoint* point1 = entity1->GetComponent<ComponentCollisionPoint>();

			if (phys1 != nullptr && point1 != nullptr)
			{
				for (Entity* entity2 : validEntities)
				{
					ComponentPhysics* phys2 = entity2->GetComponent<ComponentPhysics>();
					ComponentCollisionSphere* sphere2 = entity2->GetComponent<ComponentCollisionSphere>();

					if (entity1 != entity2 && phys2 != nullptr && sphere2 != nullptr)
					{
						CollisionCheck(entity1, entity2);
					}
				}
			}
		}


	}

	// entity 1 has the point, 2 has the circle

	virtual void CollisionCheck(Entity* p_entity_1, Entity* p_entity_2)
	{
		ComponentTransform* trans1 = p_entity_1->GetComponent<ComponentTransform>();
		ComponentCollisionPoint* point1 = p_entity_1->GetComponent<ComponentCollisionPoint>();
		vec3 point = trans1->m_translation + point1->GetPoint();

		ComponentTransform* trans2 = p_entity_2->GetComponent<ComponentTransform>();
		ComponentCollisionSphere* sphere2 = p_entity_2->GetComponent<ComponentCollisionSphere>();
		vec3 pos2 = trans2->m_translation;
		float radius = sphere2->GetCollisionSphere();

		vec3 distance = pos2 - point;
		float leng = glm::length(distance);

		if (leng < radius)
		{
			m_cm->RegisterCollision(p_entity_1, p_entity_2, AABB_SPHERE_POINT);
		}
	}

	virtual void ValidateEntity(Entity* p_entity) override
	{
		bool requiredComponents = (p_entity->GetComponent<ComponentCollisionPoint>() != nullptr &&
			p_entity->GetComponent<ComponentTransform>() != nullptr) || (p_entity->GetComponent<ComponentCollisionSphere>() != nullptr &&
				p_entity->GetComponent<ComponentTransform>() != nullptr);

		System::ValidateEntity(p_entity, requiredComponents);
	}

private:
	CollisionManager* m_cm;
};
