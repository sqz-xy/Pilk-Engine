#include "PilkEngineCommon.h"

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

std::vector<Entity> FileManager::LoadEntities(const std::string& p_filePath)
{
    //vector<std::string> array;
    //string line;

    //ifstream readFile;
    //readFile.open(p_filePath);

    //string entityString;

    //if (!readFile) 
    //{
    //    std::cout << "Error reading file";
    //}
    //else
    //{
    //    char cha;
    //    while (!readFile.eof()) 
    //    {
    //        readFile >> line;
    //        entityString += line + " ";
    //    }
    //    char sep = '&';
    //    vector<string> strings = customSplit(entityString, sep);
    //     
    //    for (int i = 1; i <= strings.size(); i++)
    //    {
    //        char sepe = ' ';
    //        char seperator = ',';
    //        vector<string> entityArray = customSplit(strings[i], sepe);
    //        entityArray.erase(entityArray.begin() + 0);
    //        Entity* newEntity = new Entity(entityArray[0].c_str());

    //        for(int y = 0; y <= entityArray.size(); y++)
    //        {
    //            if (entityArray[y] == "ComponentTransform")
    //            {
    //                vector<string> arrayVectorValue = { customSplit(entityArray[y + 1], seperator) };

    //                vector<string> translation;
    //                translation.push_back(arrayVectorValue[0]); 
    //                translation.push_back(arrayVectorValue[2]);
    //                translation.push_back(arrayVectorValue[1]);

    //                vector<string> rotation;
    //                rotation.push_back(arrayVectorValue[3]);
    //                rotation.push_back(arrayVectorValue[4]);
    //                rotation.push_back(arrayVectorValue[5]);

    //                vector<string> scale;
    //                scale.push_back(arrayVectorValue[6]);
    //                scale.push_back(arrayVectorValue[7]);
    //                scale.push_back(arrayVectorValue[8]);

    //                //newEntity->AddComponent(new ComponentTransform(static_cast<>(translation), rotation, scale));
    //            }
    //            else if (entityArray[y] == "ComponentCollisionAABB") 
    //            {
    //                vector<string> arrayVectorValue = { customSplit(entityArray[y + 1], seperator) };
    //                //newEntity->AddComponent(new ComponentCollisionAABB(static_cast<float>(arrayVectorValue[0]), arrayVectorValue[1], arrayVectorValue[2]));
    //            }
    //            else if (entityArray[y] == "ComponentCollisionSphere")
    //            {
    //                //newEntity->AddComponent(new ComponentCollisionSphere(entityArray[y + 1]));
    //            }
    //            else if (entityArray[y] == "ComponentPhysics")
    //            {
    //                vector<string> arrayVectorValue = { customSplit(entityArray[y + 1], seperator) };

    //                vector<string> velocity;
    //                velocity.push_back(arrayVectorValue[0]);
    //                velocity.push_back(arrayVectorValue[2]);
    //                velocity.push_back(arrayVectorValue[1]);

    //                vector<string> gravity;
    //                gravity.push_back(arrayVectorValue[3]);
    //                gravity.push_back(arrayVectorValue[4]);
    //                gravity.push_back(arrayVectorValue[5]);

    //                //newEntity->AddComponent(new ComponentPhysics(static_cast<float>(velocity), gravity));
    //            }
    //            else if (entityArray[y] == "ComponentGeometry")
    //            {

    //            }
    //            else if (entityArray[y] == "ComponentPhysics")
    //            {

    //            }
    //            else if (entityArray[y] == "ComponentShader")
    //            {

    //            }
    //        }
    //        
    //        cout << entityArray[i];
    //    }
    //  
    //}
    //readFile.close();

    return std::vector<Entity>();
}

