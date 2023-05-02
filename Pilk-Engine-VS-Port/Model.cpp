#include "PilkEngineCommon.h"

#include "Model.h"
#include "ResourceManager.h"

Model::Model(const char* p_filePath)
{
    loadModel(p_filePath);
}

Model::~Model()
{
    m_textures.clear();
    m_meshes.clear();
    m_directory.clear();
}

// By Thomas Beet 
// Followed https://learnopengl.com/Model-Loading/Assimp
/// <summary>
/// Tells all meshes in the model to draw
/// </summary>
/// <param name="pShaderHandle">Shader handle</param>
void Model::Draw(int pShaderHandle)
{
    for (unsigned int i = 0; i < m_meshes.size(); i++)
        m_meshes[i].Draw(pShaderHandle);
}

// By Thomas Beet 
// Followed https://learnopengl.com/Model-Loading/Assimp
/// <summary>
/// Begins the recursive model loading process, models are set up in a tree structure
/// </summary>
/// <param name="p_path">Model path</param>
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

    // Calls the next node
    processNode(scene->mRootNode, scene);
}

// By Thomas Beet 
// Followed https://learnopengl.com/Model-Loading/Assimp
/// <summary>
/// Processes the mesh stored at this node
/// </summary>
/// <param name="p_node">Current node</param>
/// <param name="p_scene">Current scene</param>
void Model::processNode(aiNode* p_node, const aiScene* p_scene)
{
    // Process meshes
    for (unsigned int i = 0; i < p_node->mNumMeshes; i++)
    {
        aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, p_scene));
    }
    // Process children (Recursive)
    for (unsigned int i = 0; i < p_node->mNumChildren; i++)
    {
        processNode(p_node->mChildren[i], p_scene);
    }
}

/// <summary>
/// Gets all the vertex data from a mesh
/// </summary>
/// <param name="p_mesh">The current mesh</param>
/// <param name="p_scene">The current scene</param>
/// <returns>A model object</returns>
Geometry Model::processMesh(aiMesh* p_mesh, const aiScene* p_scene)
{
    // Data storage
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Vertices
    for (unsigned int i = 0; i < p_mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; 

        vector.x = p_mesh->mVertices[i].x;
        vector.y = p_mesh->mVertices[i].y;
        vector.z = p_mesh->mVertices[i].z;
        vertex.Position = vector;
      
        // Normals
        if (p_mesh->HasNormals())
        {
            vector.x = p_mesh->mNormals[i].x;
            vector.y = p_mesh->mNormals[i].y;
            vector.z = p_mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // Tex coords
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

    // Indices
    for (unsigned int i = 0; i < p_mesh->mNumFaces; i++)
    {
        aiFace face = p_mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Materials
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

// By Thomas Beet 
// Followed https://learnopengl.com/Model-Loading/Assimp
/// <summary>
/// Loads material textures for a mesh
/// </summary>
/// <param name="p_mat">Material</param>
/// <param name="p_type">Material type</param>
/// <param name="p_typeName">type in string form</param>
/// <returns>List of mesh textures</returns>
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
            texture.id = ResourceManager::LoadTexture(str.C_Str(), m_directory); // Load texture using resource manager
            texture.type = p_typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            m_textures.push_back(texture);
        }
    }
    return textures;
}

void Model::ClearData()
{
    for (unsigned int i = 0; i < m_meshes.size(); i++)
        m_meshes[i].ClearData();
}
