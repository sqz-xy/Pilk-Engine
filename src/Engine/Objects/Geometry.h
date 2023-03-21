#pragma once

#include "../PilkEngineCommon.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// https://ogldev.org/www/tutorial22/tutorial22.html

class Geometry
{
public:
    Geometry();

    ~Geometry();

    bool LoadMesh(const std::string& p_filename);

    void Render();

private:
    bool InitFromScene(const aiScene* p_scene, const std::string& p_filename);
    void InitMesh(unsigned int p_index, const aiMesh* p_aiMesh);
    bool InitMaterials(const aiScene* p_scene, const std::string& p_filename);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

    struct MeshEntry {
        MeshEntry();

        ~MeshEntry();

        bool Init(const std::vector& Vertices,
            const std::vector& Indices);

        GLuint VB;
        GLuint IB;
        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };

    std::vector m_Entries;
    std::vector m_Textures;
};