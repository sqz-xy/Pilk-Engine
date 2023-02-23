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

	glm::mat4 m_modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
	Camera* m_camera;

	/*float m_vertices[9]
	{
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f
	};*/

		float m_vertices[72]
		{
				-0.2f, -0.2f, -0.2f,
				0.2f, -0.2f, -0.2f,
				-0.2f, 0.2f, -0.2f,
				0.2f, 0.2f, -0.2f,
				-0.2f, -0.2f, 0.2f,
				0.2f, -0.2f, 0.2f,
				-0.2f, 0.2f, 0.2f,
				0.2f, 0.2f, 0.2f,
				0.2f, -0.2f, -0.2f,
				0.2f, -0.2f, 0.2f,
				0.2f, 0.2f, -0.2f,
				0.2f, 0.2f, 0.2f,
				-0.2f, -0.2f, -0.2f,
				-0.2f, -0.2f, 0.2f,
				-0.2f, 0.2f, -0.2f,
				-0.2f, 0.2f, 0.2f,
				-0.2f, -0.2f, -0.2f,
				-0.2f, -0.2f, 0.2f,
				0.2f, -0.2f, -0.2f,
				0.2f, -0.2f, 0.2f,
				-0.2f, 0.2f, -0.2f,
				-0.2f, 0.2f, 0.2f,
				0.2f, 0.2f, -0.2f,
				0.2f, 0.2f, 0.2f
		};

	/*	unsigned int m_indices[6]
	{
		0,1,3,
		1,2,3
	};*/
		float m_indices[39]
	{
			 1, 0, 2,
			1, 2, 3,
			4, 5, 6,
			6, 5, 7,
			9, 8, 10,
			9, 10, 11,
			12, 13, 14,
			14, 13, 15,
			17, 16, 18,
			17, 18, 19,
			20, 21, 22,
			22, 21, 23
	};

	explicit MainMenuScene(SceneManager* pSceneManager) : Scene(pSceneManager)
	{
		m_sceneManager->m_windowName = "MainMenuScene";
		m_camera = new Camera(glm::vec3(0.0f,0.0f,2.0f), glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, m_sceneManager->m_width, m_sceneManager->m_height);
	}

	~MainMenuScene() override
	{
		this->Close();
	}

	void Render(const float p_dt) const override
	{
		glUseProgram(m_shaderProgramID);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 39);

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

		glUseProgram(m_shaderProgramID);
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, "uModel"), 1, GL_FALSE, &m_modelMat[0][0]);
		glUniform4f(glGetUniformLocation(m_shaderProgramID, "uColour"), m_colour[0], m_colour[1], m_colour[2], m_colour[3]);
		m_camera->UpdateCamera(m_shaderProgramID);
	}

	void processInput(GLFWwindow* p_window) override
	{
		// Exit if escape key is pressed
		if (glfwGetKey(p_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(p_window, true);
	}

	void Load() override
	{
		std::cout << "Scene Loaded" << std::endl;

		// TODO: Refactor vertices and indices into geometry component, Refactor shader into shader component
		
		unsigned int VBO, VAO, EBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);

		// Creates a shader program
		unsigned int shaderProgram;
		if (!ResourceManager::CreateShaderProgram(&shaderProgram, "resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag")) return;

		// Buffer Data
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		m_vao = VAO;
		m_shaderProgramID = shaderProgram;
	}

	void ProcessInput(GLFWwindow* p_window, const float p_dt) override
	{
		if (glfwGetKey(p_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(p_window, true);

		if (glfwGetKey(p_window, GLFW_KEY_W) == GLFW_PRESS)
			m_camera->MoveCamera(m_camera->Forward, 2.5f, p_dt);

		if (glfwGetKey(p_window, GLFW_KEY_S) == GLFW_PRESS)
			m_camera->MoveCamera(m_camera->Backward, 2.5f, p_dt);

		if (glfwGetKey(p_window, GLFW_KEY_A) == GLFW_PRESS)
			m_camera->MoveCamera(m_camera->Left, 2.5f, p_dt);

		if (glfwGetKey(p_window, GLFW_KEY_D) == GLFW_PRESS)
			m_camera->MoveCamera(m_camera->Right, 2.5f, p_dt);
	}

	void Close() override
	{
		std::cout << "Scene Closed" << std::endl;
		ResourceManager::DeleteResources();
	}
};