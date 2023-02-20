#pragma once

#include "ResourceManager.h"

#include "iostream"
#include "fstream"


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

int ResourceManager::CreateShaderProgram(unsigned int* p_sProgram, const char* p_vertFileName, const char* p_fragFileName)
{
	int success;
	char infoLog[512];

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
	return true;
}
