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
    std::vector<std::string> array;
    std::string line;

    std::ifstream readFile;
    readFile.open(p_filePath);

    std::string arr;
    std::string entityString;

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
         
        for (int i = 1; i < strings.size(); i++)
        {
            char sepe = ' ';
            vector<string> entityArray = customSplit(strings[i], sepe);
            cout << entityArray[i];
        }
      
    }
    readFile.close();

    return std::vector<Entity>();
}

