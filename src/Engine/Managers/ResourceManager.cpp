#pragma once

#include "ResourceManager.h"

#include "map"
#include "iostream"
#include "fstream"
#include "string"
#include "errno.h"

std::map<std::string, unsigned int*> ResourceManager::m_shaderMap;

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
		std::cout << p_shaderType << " shader compilation error" << std::endl;
		return false;
	}
	return true;
}


bool ResourceManager::CreateShaderProgram(unsigned int* p_sProgram, const char* p_vertFileName, const char* p_fragFileName)
{
	// Shader signature is the vertex + fragment filenames appended
	
	// Preallocate buffer to store shader signature
	const int bufferSize = 512;
	char* shaderSig = new char [bufferSize];

	strcpy_s(shaderSig, bufferSize,p_vertFileName);
	strcat_s(shaderSig, bufferSize, p_fragFileName);
	
	// If the shader already exists, return its program id
	std::size_t shaderCount = m_shaderMap.count(static_cast<std::string>(shaderSig)), shaderLimit = 1;
	if (shaderCount == shaderLimit)
	{
		*p_sProgram = *m_shaderMap.at(static_cast<std::string>(shaderSig));
		return true;
	}

	int success;
	char infoLog[bufferSize];

	unsigned int vertexShader;
	unsigned int fragmentShader;

	CompileShader(GL_VERTEX_SHADER, p_vertFileName, &vertexShader, &success, infoLog);
	CompileShader(GL_FRAGMENT_SHADER, p_fragFileName, &fragmentShader, &success, infoLog);

	*p_sProgram = glCreateProgram();

	glAttachShader(*p_sProgram, vertexShader);
	glAttachShader(*p_sProgram, fragmentShader);
	glLinkProgram(*p_sProgram);

	glGetProgramiv(*p_sProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(*p_sProgram, 512, NULL, infoLog);
		std::cout << "Shader program attachment error" << std::endl;
		return false;
	}

	// Add the shader to the map
	m_shaderMap.insert(std::pair <std::string, unsigned int* > (static_cast<std::string>(shaderSig), p_sProgram));

	return true;
}

void ResourceManager::DeleteResources()
{
	for (const auto& shader : m_shaderMap) {
		glDeleteProgram(*shader.second);
	}
	m_shaderMap.clear();
}
