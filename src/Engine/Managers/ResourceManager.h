#pragma once

#include "glad.h"

#include "string"
#include "map"

const int max_shaders = 20;

class ResourceManager final
{
public:
	explicit ResourceManager() = delete;
	static bool CreateShaderProgram(unsigned int* p_sProgram, const char* p_vertFileName, const char* p_fragFileName);
	static void DeleteResources();

private:
	static bool LoadShader(const char* p_fileName, std::string& p_shaderSource);
	static bool CompileShader(const GLenum& p_shaderType, const char* p_fileName, unsigned int* p_shaderBuffer, int* p_success, char* p_infoLog);
	
	static std::map<std::string, unsigned int*> m_shaderMap;
};