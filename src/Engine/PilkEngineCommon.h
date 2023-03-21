#pragma once

// GLM
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

// STD
#include "iostream"
#include "fstream"
#include "string"
#include "cerrno"
#include "ostream"
#include "vector"
#include "map"

// OpenGL/GLFW
#include "glad.h"
#include "glfw3.h"

// ImGUI
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

// Assimp
#include "assimp/Importer.hpp"
#include "assimp/scene.h"    
#include "assimp/postprocess.h" 

// Variables
#define MOUSE_CONTROL_ENABLED 1;