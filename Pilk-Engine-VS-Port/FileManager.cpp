#include "PilkEngineCommon.h"

std::vector<Entity> FileManager::LoadEntities(const std::string& p_filePath)
{
    std::vector<std::string> array;
    std::string line;

    std::ifstream readFile;
    readFile.open(p_filePath);

    if (readFile.is_open()) 
    {

        while (getline(readFile, line)) 
        {
            std::cout << line << std::endl;
        }
    }

    return std::vector<Entity>();
}

