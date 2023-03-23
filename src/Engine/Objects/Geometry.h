#pragma once

#include "../PilkEngineCommon.h"

// Written by Thomas Beet, done by following this model loading tutorial. Code has been changed to fit with this engine
// https://learnopengl.com/Model-Loading/Assimp

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

// By Thomas Beet 
// Followed https://learnopengl.com/Model-Loading/Assimp
class Geometry
{
public:
	explicit Geometry(std::vector<Vertex> p_vertices, std::vector<unsigned int> p_indices, std::vector<Texture> p_textures);
	~Geometry();

	void Draw(int pShaderHandle);
	void ClearData();

	std::vector<Vertex> m_vertices;
	std::vector<Texture> m_textures;
	std::vector<unsigned int> m_indices;

private:
	void SetupMesh();

	unsigned int m_vao, m_vbo, m_ebo;
};
