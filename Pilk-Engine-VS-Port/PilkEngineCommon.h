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
#include "filesystem" // Swapped to C++ 17 from 14

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

// STB
#include "stb_image.h"

// Internal Headers

// Managers
#include "CollisionManager.h"
#include "EntityManager.h"
#include "FileManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "SystemManager.h"
#include "PrefabManager.h"

// Entity
#include "Entity.h"
#include "Component.h"

// Objects
#include "Camera.h"
#include "Geometry.h"
#include "Model.h"

// System
#include "System.h"

// Scenes
#include "Scene.h"
#include "SceneTypes.h"

// Variables
#define MOUSE_CONTROL_ENABLED 0;
#define GRAVITY glm::vec3(0.0f, 9.8f, 0.0f);