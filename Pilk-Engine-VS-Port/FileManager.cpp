#pragma once

#include "PilkEngineCommon.h"

#include "FileManager.h"
#include "Entity.h"

using namespace std;


/// <summary>
/// https://favtutor.com/blogs/split-string-cpp
/// </summary>
/// <param name="str"></param>
/// <param name="separator"></param>
vector<string> customSplit(string& str, char separator) {
    vector<string> strings;
    int startIndex = 0, endIndex = 0;
    for (int i = 0; i <= str.size(); i++) {

        // If we reached the end of the word or the end of the input.
        if (str[i] == separator || i == str.size()) {
            endIndex = i;
            string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            strings.push_back(temp);
            startIndex = endIndex + 1;
        }
    }
    return strings;
}

std::vector<Entity*> FileManager::LoadEntities(const std::string& p_filePath)
{
    vector<Entity*> entityList;
    vector<std::string> array;
    string line;

    ifstream readFile;
    readFile.open(p_filePath);

    string entityString;

    if (!readFile) 
    {
        std::cout << "Error reading file";
    }
    else
    {
        char cha;
        while (!readFile.eof()) 
        {
            readFile >> line;
            entityString += line + " ";
        }
        char sep = '&';
        vector<string> strings = customSplit(entityString, sep);
         
        for (int i = 1; i <= strings.size() - 1; i++)
        {
            char sepe = ' ';
            char seperator = ',';
            vector<string> entityArray = customSplit(strings[i], sepe);
            entityArray.erase(entityArray.begin() + 0);

            //strcpy
            const int returnBufferSize = 512;
            char* name = new char[returnBufferSize];
            strcpy_s(name, returnBufferSize, entityArray[0].c_str());

            Entity* newEntity = new Entity(name);

            for(int y = 0; y <= entityArray.size() - 1; y++)
            {
                if (entityArray[y] == "ComponentTransform")
                {
                    vector<string> arrayVectorValue = { customSplit(entityArray[y + 1], seperator) };

                    glm::vec3 trans(std::stof(arrayVectorValue[0].c_str()), std::stof(arrayVectorValue[1].c_str()), std::stof(arrayVectorValue[2].c_str()));
                    glm::vec3 rot(std::stof(arrayVectorValue[3].c_str()), std::stof(arrayVectorValue[4].c_str()), std::stof(arrayVectorValue[5].c_str()));
                    glm::vec3 scale(std::stof(arrayVectorValue[6].c_str()), std::stof(arrayVectorValue[7].c_str()), std::stof(arrayVectorValue[8].c_str()));

                    newEntity->AddComponent(new ComponentTransform(trans, rot, scale));
                }
                else if (entityArray[y] == "ComponentCollisionAABB") 
                {
                    vector<string> arrayVectorValue = { customSplit(entityArray[y + 1], seperator) };
                    newEntity->AddComponent(new ComponentCollisionAABB(stof(arrayVectorValue[0].c_str()), stof(arrayVectorValue[1].c_str()), atof(arrayVectorValue[2].c_str())));
                }
                else if (entityArray[y] == "ComponentCollisionSphere")
                {
                    newEntity->AddComponent(new ComponentCollisionSphere(stof(entityArray[y + 1].c_str())));
                }
                else if (entityArray[y] == "ComponentPhysics")
                {
                    vector<string> arrayVectorValue = { customSplit(entityArray[y + 1], seperator) };

                    int vel1 = std::stof(arrayVectorValue[0].c_str()), vel2 = std::stof(arrayVectorValue[1].c_str()), vel3 = std::stof(arrayVectorValue[2].c_str());
                    int grav1 = std::stof(arrayVectorValue[3].c_str()), grav2 = std::stof(arrayVectorValue[4].c_str()), grav3 = std::stof(arrayVectorValue[5].c_str());

                    glm::vec3 velocity(vel1, vel2, vel3);
                    glm::vec3 gravity(grav1, grav2, grav3);

                    newEntity->AddComponent(new ComponentPhysics(velocity, gravity));
                }
                else if (entityArray[y] == "ComponentGeometry")
                {
                    newEntity->AddComponent(new ComponentGeometry(entityArray[y + 1].c_str()));
                }
                else if (entityArray[y] == "ComponentShader")
                {
                    vector<string> arrayVectorValue = { customSplit(entityArray[y + 1], seperator) };
                    newEntity->AddComponent(new ComponentShader(arrayVectorValue[0].c_str(), arrayVectorValue[1].c_str()));
                }
            }
            entityList.push_back(newEntity);
        }
      
    }
    readFile.close();

    return entityList;
}

