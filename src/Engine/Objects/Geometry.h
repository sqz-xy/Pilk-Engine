#pragma once

#include "../PilkEngineCommon.h"

// https://ogldev.org/www/tutorial22/tutorial22.html
// https://learnopengl.com/Model-Loading/Mesh

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
};

class Geometry
{
public:
	explicit Geometry(std::vector<Vertex> p_vertices, std::vector<unsigned int> p_indices, std::vector<Texture> p_textures);

	void Draw(int pShaderHandle);

	std::vector<Vertex> m_vertices;
	std::vector<Texture> m_textures;
	std::vector<unsigned int> m_indices;

private:
	void SetupMesh();

	unsigned int m_vao, m_vbo, m_ebo;
};
