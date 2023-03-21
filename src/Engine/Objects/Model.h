#pragma once
#include "../PilkEngineCommon.h"
#include "Geometry.h"

//TODO: THOMAS BEET: Refactor texture loading to the resource manager, remove it from the model. Make sure models are sent to the output directory

class Model
{
public:
    explicit Model(char* p_filePath);

    void Draw(int pShaderHandle);

private:
    void loadModel(std::string p_path);
    void processNode(aiNode* p_node, const aiScene* p_scene);
    Geometry processMesh(aiMesh* p_mesh, const aiScene* p_scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* p_mat, aiTextureType p_type,std::string p_typeName);
    unsigned int TextureFromFile(const char* path, const std::string& p_directory);

    // model data
    std::vector<Geometry> m_meshes;
    std::vector<Texture> m_textures;
    std::string m_directory;
};