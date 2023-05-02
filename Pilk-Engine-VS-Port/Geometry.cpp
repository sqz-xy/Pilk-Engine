#pragma once

#include "PilkEngineCommon.h"

#include "Geometry.h"

Geometry::Geometry(std::vector<Vertex> p_vertices, std::vector<unsigned int> p_indices, std::vector<Texture> p_textures)
{
	m_vertices = p_vertices;
	m_textures = p_textures;
	m_indices = p_indices;

	SetupMesh();
}

Geometry::~Geometry()
{
    m_vertices.clear();
    m_textures.clear();
    m_indices.clear();
}

/// <summary>
/// Generates buffers and binds the vertex/index data. 
/// </summary>
void Geometry::SetupMesh()
{
    // Used previous work and tutorial as reference 
    // https://learnopengl.com/Model-Loading/Assimp

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    // Bind vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    // Bind indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

    // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // Tex coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

// By Thomas Beet using https://learnopengl.com/Model-Loading/Assimp
/// <summary>
/// Draws the geometry
/// </summary>
/// <param name="pShaderHandle">Shader buffer</param>
void Geometry::Draw(int pShaderHandle)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    // Handle materials, could be a problem child in the future
    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); 
        
        // Handles texture sampler uniforms in the shader, they should be names uTextureDiffuse1++ / uTextureSpecular1++
        std::string number;
        std::string name = m_textures[i].type;
        if (name == "uTextureDiffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "uTextureSpecular")
            number = std::to_string(specularNr++);

        std::string uniformName = name + number;
        glUniform1i(glGetUniformLocation(pShaderHandle, uniformName.c_str()), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Deletes buffers
void Geometry::ClearData()
{
    //TODO: Fix
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_ebo);
}
