#include <vector>
#include "string"
#include "map"
#include "Engine\src\Engine\Components\Component.h"

#pragma once
class ComponentManager final
{

public:
	//idk what this needs as a parameter lol
	explicit ComponentManager();

private:

	//private methods go here

	//data members
	// I am assuming the ID will be a string? Makes sense as a human readable way to identify what the entity is.

	std::map<ComponentPosition, std::string> mComponentPositions;
	std::map<ComponentShader, std::string> mComponentShaders;
};