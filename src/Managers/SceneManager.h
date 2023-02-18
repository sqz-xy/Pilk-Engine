#pragma once

#include "glad.h"
#include "glfw3.h"

class SceneManager
{

	// Constructor takes in the dimensions, store dimensions and window name

private:
	void Render(const float dt) const;
	void Update(const float dt);
	void Load();
	void Run();

};
