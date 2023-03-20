#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <vector>
#include <string>

class Entity
{
public:
	/// <summary>
	/// Constructs an Entity.
	/// <para> Original Author: Piotr Moskala. </para>
	/// </summary>
	/// <param name="p_name"> The Entity's name. </param>
	explicit Entity(char* p_name)
	{
		m_name = p_name;
	}

	/// <summary>
	/// Returns the name of the Entity.
	/// <para> Original Author: Piotr Moskala. </para>
	/// </summary>
	char* GetName()
	{
		return m_name;
	}

	/// <summary>
	/// Adds the specified Component pointer to the Entity's vector of Component pointers.
	/// <para> Original Author: Piotr Moskala. </para>
	/// </summary>
	/// <param name="p_component"> The Component pointer to add. </param>
	void AddComponent(Component* p_component)
	{
		m_components.push_back(p_component);
	}

	/// <summary>
	/// Returns the Component pointer of the specified type if one exists in the Entity.
	/// <para> Original Author: Piotr Moskala. </para>
	/// </summary>
	/// <typeparam name="T"> The type of Component to check for. </typeparam>
	/// <returns> The Component pointer of the specified type, or nullptr if there is no match. </returns>
	template<typename T>
	T* GetComponent()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			T* result = dynamic_cast<T*>(m_components[i]);
			if (result != nullptr)
			{
				return result;
			}
		}
		return nullptr;
	}

private:
	char* m_name;
	std::vector<Component*> m_components;
};