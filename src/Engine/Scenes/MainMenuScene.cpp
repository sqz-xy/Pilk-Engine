#pragma once

#include "Scene.h"

#include "imgui_impl_opengl3.h"

#include "iostream"
#include "iostream"
#include "ostream"
#include "fstream"
#include "string"


class MainMenuScene : public Scene
{
public:

	// TODO: Remove after refactor
	int m_vao;
	int m_shaderProgramID;
	float* m_colour = new float[4] { 1.0f, 0.5f, 0.2f, 1.0f };

	explicit MainMenuScene(SceneManager* pSceneManager) : Scene(pSceneManager)
	{
		m_sceneManager->m_windowName = "MainMenuScene";
	}

	virtual ~MainMenuScene() override 
	{
		this->Close();
	}

	// TODO: Put shader loading and compiling code in the resource manager
	/**
	* \brief Loads a shader file
	* \param pFileName The name of the shader file to load
	* \param pShaderSource A reference The source code of the shader
	* \return A bool representing if the operation was successful
	*/
	bool MainMenuScene::LoadShader(const char* pFileName, std::string& pShaderSource)
	{
		pShaderSource.clear();
		std::ifstream shader(pFileName);

		if (shader)
		{
			char ch;
			while (shader.get(ch))
			{
				pShaderSource += ch;
			}
			return true;
		}
		return false;
	}

	/**
	* \brief Compiles a shader
	* \param pShaderType The type of shader
	* \param pFileName The shaders file name
	* \param pShaderBuffer The shaders buffer
	* \param pSuccess The success status
	* \param pInfoLog The info log
	* \return Boolean value representing if the operation was successful
	*/
	bool MainMenuScene::compile_shader(const GLenum& pShaderType, const char* pFileName, unsigned int* pShaderBuffer, int* pSuccess, char* pInfoLog)
	{
		std::string shaderSourceString;
		const char* shaderSource;

		if (!LoadShader(pFileName, shaderSourceString))
		{
			std::cout << pShaderType << " File not loaded" << std::endl;
			return false;
		}

		shaderSource = shaderSourceString.c_str();
		*pShaderBuffer = glCreateShader(pShaderType);
		glShaderSource(*pShaderBuffer, 1, &shaderSource, NULL);
		glCompileShader(*pShaderBuffer);

		glGetShaderiv(*pShaderBuffer, GL_COMPILE_STATUS, pSuccess);
		if (!pSuccess)
		{
			glGetShaderInfoLog(*pShaderBuffer, 512, NULL, pInfoLog);
			std::cout << pShaderType << " shader compilation error" << std::endl;
			return false;
		}
		return true;
	}

	/**
		* \brief Creates a shader program
		* \param pVertexShader - The vertex shader handle
		* \param pFragmentShader - The fragment shader handle
		* \param pShaderProgram - The shader program handle
		* \return A bool representing if the operation was successful
		*/
	bool MainMenuScene::create_shader_program(unsigned int* pVertexShader, unsigned int* pFragmentShader, unsigned int* pShaderProgram)
	{
		int success;
		char infoLog[512];

		compile_shader(GL_VERTEX_SHADER, "resources/shaders/VertexShader.vert", pVertexShader, &success, infoLog);
		compile_shader(GL_FRAGMENT_SHADER, "resources/shaders/FragmentShader.frag", pFragmentShader, &success, infoLog);

		*pShaderProgram = glCreateProgram();

		glAttachShader(*pShaderProgram, *pVertexShader);
		glAttachShader(*pShaderProgram, *pFragmentShader);
		glLinkProgram(*pShaderProgram);

		glGetProgramiv(*pShaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(*pShaderProgram, 512, NULL, infoLog);
			std::cout << "Shader program attachment error" << std::endl;
			return false;
		}
		return true;
	}

	virtual void Render(const float p_dt) const override
	{
		glUseProgram(m_shaderProgramID);

		
		// Modify shader uniforms

		glUniform4f(glGetUniformLocation(m_shaderProgramID, "uColour"), m_colour[0], m_colour[1], m_colour[2], m_colour[3]);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		ImVec2 vec(100, 50);
		//std::cout << "Rendering" << std::endl;
		ImGui::Begin("ImGui Test");
		ImGui::Text("Delta time %f", p_dt);
		ImGui::ColorEdit4("Colour", m_colour);
		bool button = ImGui::Button("Change scene", vec);

		if (button)
			m_sceneManager->ChangeScene(MainMenu);

		ImGui::End();
	}

	virtual void Update(const float p_dt) override
	{
		//std::cout << "Updating" << std::endl;
	}

	virtual void Load() override
	{
		std::cout << "Scene Loaded" << std::endl;

		// TODO: Refactor vertices and indices into geometry component, Refactor shader into shader component
		float vertices[] =
		{
				-0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f,
				0.0f,  0.5f, 0.0f
		};

		float indices[] =
		{
				0, 1, 2
		};

		unsigned int VBO, VAO, EBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);

		// Creates a shader program
		unsigned int vertexShader, fragmentShader, shaderProgram;
		if (!create_shader_program(&vertexShader, &fragmentShader, &shaderProgram)) return;

		// Buffer Data
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		m_vao = VAO;
		m_shaderProgramID = shaderProgram;	
	}

	virtual void Close() override
	{
		std::cout << "Scene Closed" << std::endl;
	}
};