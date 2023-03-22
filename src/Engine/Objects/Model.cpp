#include "Model.h"
#include "Model.h"
#include "../PilkEngineCommon.h"
#include "../Managers/ResourceManager.h"

Model::Model(char* p_filePath)
{
    loadModel(p_filePath);
}

Model::~Model()
{
    m_textures.clear();
    m_meshes.clear();
    m_directory.clear();
}

void Model::Draw(int pShaderHandle)
{
    for (unsigned int i = 0; i < m_meshes.size(); i++)
        m_meshes[i].Draw(pShaderHandle);
}

void Model::loadModel(std::string p_path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(p_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    m_directory = p_path.substr(0, p_path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* p_node, const aiScene* p_scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < p_node->mNumMeshes; i++)
    {
        aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, p_scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < p_node->mNumChildren; i++)
    {
        processNode(p_node->mChildren[i], p_scene);
    }
}

Geometry Model::processMesh(aiMesh* p_mesh, const aiScene* p_scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < p_mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; 

        vector.x = p_mesh->mVertices[i].x;
        vector.y = p_mesh->mVertices[i].y;
        vector.z = p_mesh->mVertices[i].z;
        vertex.Position = vector;
      
        // normals
        if (p_mesh->HasNormals())
        {
            vector.x = p_mesh->mNormals[i].x;
            vector.y = p_mesh->mNormals[i].y;
            vector.z = p_mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (p_mesh->mTextureCoords[0]) 
        {
            glm::vec2 vec;
            vec.x = p_mesh->mTextureCoords[0][i].x;
            vec.y = p_mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // indices.
    for (unsigned int i = 0; i < p_mesh->mNumFaces; i++)
    {
        aiFace face = p_mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (p_mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = p_scene->mMaterials[p_mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "uTextureDiffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "uTextureSpecular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Geometry(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* p_mat, aiTextureType p_type, std::string p_typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < p_mat->GetTextureCount(p_type); i++)
    {
        aiString str;
        p_mat->GetTexture(p_type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < m_textures.size(); j++)
        {
            if (std::strcmp(m_textures[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(m_textures[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            //ResourceManager::LoadTexture(str.C_Str(), m_directory, texture.id);
            texture.id = TextureFromFile(str.C_Str(), m_directory);
            texture.type = p_typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            m_textures.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}


/*
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* p_mat, aiTextureType p_type, std::string p_typeName)
{
    std::vector<Texture> textures;
    unsigned int textureID = 0;
    for (unsigned int i = 0; i < p_mat->GetTextureCount(p_type); i++)
    {
        aiString str;
        p_mat->GetTexture(p_type, i, &str);
        Texture texture;
        ResourceManager::LoadTexture(str.C_Str(), m_directory, textureID);
        texture.id = textureID;
        texture.type = p_type;
    }
    return textures;
}

*/

unsigned int Model::TextureFromFile(const char* path, const std::string& p_directory)
{
    std::string filename = std::string(path);
    filename = p_directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

