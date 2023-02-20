#pragma once

#include "ResourceManager.h"

#include "map"
#include "iostream"
#include "fstream"
#include "string"
#include "errno.h"

std::map<std::string, unsigned int*> ResourceManager::m_shaderMap;

/// <summary>
/// Loads string data for a shader file
/// </summary>
/// <param name="p_fileName">path for the shader file</param>
/// <param name="p_shaderSource">string source to store the shader</param>
/// <returns>bool - success</returns>
bool ResourceManager::LoadShader(const char* p_fileName, std::string& p_shaderSource)
{
	p_shaderSource.clear();
	std::ifstream shader(p_fileName);

	if (shader)
	{
		char ch;
		while (shader.get(ch))
		{
			p_shaderSource += ch;
		}
		return true;
	}
	return false;
}

/// <summary>
/// Compiles a shader 
/// </summary>
/// <param name="p_shaderType">Type of shader</param>
/// <param name="p_fileName">File path for the shader</param>
/// <param name="p_shaderBuffer">Buffer int for shader</param>
/// <param name="p_success">return code</param>
/// <param name="p_infoLog">information on shader compilation</param>
/// <returns>bool - success</returns>
bool ResourceManager::CompileShader(const GLenum& p_shaderType, const char* p_fileName, unsigned int* p_shaderBuffer, int* p_success, char* p_infoLog)
{
	std::string shaderSourceString;
	const char* shaderSource;

	if (!LoadShader(p_fileName, shaderSourceString))
	{
		std::cout << p_shaderType << " File not loaded" << std::endl;
		return false;
	}

	shaderSource = shaderSourceString.c_str();
	*p_shaderBuffer = glCreateShader(p_shaderType);
	glShaderSource(*p_shaderBuffer, 1, &shaderSource, NULL);
	glCompileShader(*p_shaderBuffer);

	glGetShaderiv(*p_shaderBuffer, GL_COMPILE_STATUS, p_success);
	if (!p_success)
	{
		glGetShaderInfoLog(*p_shaderBuffer, 512, NULL, p_infoLog);
		std::cout << p_shaderType << "shader compilation error " << std::endl << p_infoLog << std::endl;
		return false;
	}
	return true;
}

/// <summary>
/// Creates a shader program using the passed in shader paths
/// </summary>
/// <param name="p_sProgram">int to hold the program ID</param>
/// <param name="p_vertFileName">vertex shader file path</param>
/// <param name="p_fragFileName">fragment shader file path</param>
/// <returns></returns>
bool ResourceManager::CreateShaderProgram(unsigned int* p_sProgram, const char* p_vertFileName, const char* p_fragFileName)
{
	// Shader signature is the vertex + fragment filenames appended
	// Preallocate buffer to store shader signature

	const int returnBufferSize = 512;
	char* shaderSig = new char [returnBufferSize];

	strcpy_s(shaderSig, returnBufferSize,p_vertFileName);
	strcat_s(shaderSig, returnBufferSize, p_fragFileName);
	
	// If the shader already exists, return its program id
	const std::size_t shaderCount = m_shaderMap.count(static_cast<std::string>(shaderSig)), shaderLimit = 1;
	if (shaderCount == shaderLimit)
	{
		*p_sProgram = *m_shaderMap.at(static_cast<std::string>(shaderSig));
		return true;
	}

	int success;
	char infoLog[returnBufferSize];

	unsigned int vertexShader, fragmentShader;

	// Compile and attach shaders
	CompileShader(GL_VERTEX_SHADER, p_vertFileName, &vertexShader, &success, infoLog);
	CompileShader(GL_FRAGMENT_SHADER, p_fragFileName, &fragmentShader, &success, infoLog);

	*p_sProgram = glCreateProgram();

	glAttachShader(*p_sProgram, vertexShader);
	glAttachShader(*p_sProgram, fragmentShader);
	glLinkProgram(*p_sProgram);

	glGetProgramiv(*p_sProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(*p_sProgram, returnBufferSize, NULL, infoLog);
		std::cout << "Shader program attachment error " << std::endl << infoLog << std::endl;
		return false;
	}

	// Add the shader to the map
	m_shaderMap.insert(std::pair <std::string, unsigned int* > (static_cast<std::string>(shaderSig), p_sProgram));

	return true;
}

/// <summary>
/// Delete stored resources
/// </summary>
void ResourceManager::DeleteResources()
{
	for (const auto& shader : m_shaderMap) {
		glDeleteProgram(*shader.second);
	}
	m_shaderMap.clear();
}
