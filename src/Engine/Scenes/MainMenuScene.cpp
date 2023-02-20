#pragma once

#include "Scene.h"

#include "imgui_impl_opengl3.h"

#include "iostream"
#include "ostream"
#include "string"
#include "../Managers/ResourceManager.h"


class MainMenuScene : public Scene
{
public:

	// TODO: Remove after refactor
	unsigned int m_vao;
	unsigned int m_shaderProgramID;
	float* m_colour = new float[4] { 1.0f, 0.5f, 0.2f, 1.0f };

	explicit MainMenuScene(SceneManager* pSceneManager) : Scene(pSceneManager)
	{
		m_sceneManager->m_windowName = "MainMenuScene";
	}

	~MainMenuScene() override
	{
		this->Close();
	}

	void Render(const float p_dt) const override
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

	void Update(const float p_dt) override
	{
		//std::cout << "Updating" << std::endl;
	}

	void Load() override
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
		unsigned int shaderProgram;
		if (!ResourceManager::CreateShaderProgram(&shaderProgram, "resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag")) return;

		// Buffer Data
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		m_vao = VAO;
		m_shaderProgramID = shaderProgram;	
	}

    void Close() override
	{
		std::cout << "Scene Closed" << std::endl;
		ResourceManager::DeleteResources();
	}
};