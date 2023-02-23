#pragma once

#include "Scene.h"

#include "imgui_impl_opengl3.h"

#include "iostream"
#include "ostream"
#include "string"

#include "../Managers/ResourceManager.h"
#include "../Utility/Camera.h"	


class MainMenuScene : public Scene
{
public:

	// TODO: Remove after refactor
	unsigned int m_vao;
	unsigned int m_shaderProgramID;
	float* m_colour = new float[4] { 1.0f, 0.5f, 0.2f, 1.0f };

	glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, -2.0f);

	glm::mat4 m_modelMat;
	
	
	Camera* m_Camera;



	float m_vertices[72]
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	float m_indices[39]
	{
		0, 1, 2
	};


	explicit MainMenuScene(SceneManager* pSceneManager) : Scene(pSceneManager)
	{
		m_sceneManager->m_windowName = "MainMenuScene";
		
		m_Camera = new Camera(	glm::vec3(0.0f,0.0f,2.0f),	// camPos
								glm::vec3(0.0f,0.0f,0.0f),  // camTarget
								glm::vec3(glm::vec3(0.0f, 0.0f, -2.0f) - glm::vec3(0.0f,0.0f,0.0f)), // cam direction 
								glm::vec3(0.0f,1.0f,0.0f),	// cam up
								m_sceneManager->m_width,	// windows width
								m_sceneManager->m_height);	// window height


		// rotation
		// scale
		m_modelMat = glm::rotate(m_modelMat, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		m_modelMat *= glm::translate(glm::mat4(1.0f), modelPos); // translatng the model in the matrix
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
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, "uModel"), 1, GL_FALSE, &m_modelMat[0][0]);
		m_Camera->UpdateCamera(m_shaderProgramID);


		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		ImVec2 vec(100, 50);
		//std::cout << "Rendering" << std::endl;
		ImGui::Begin("ImGui Test");
		ImGui::Text("Delta time %f", p_dt);
		ImGui::ColorEdit4("Colour", m_colour);
		bool button = ImGui::Button("Change scene", vec);

		if (button)
			m_sceneManager->ChangeScene(MainMenu);


		
		glUseProgram(0);

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
		// Exit if escape key is pressed
		if (glfwGetKey(p_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(p_window, true);

		if (glfwGetKey(p_window, GLFW_KEY_W) == GLFW_PRESS)
			m_Camera->MoveCamera(m_Camera->Forward, 2.5f, p_dt);

		if (glfwGetKey(p_window, GLFW_KEY_S) == GLFW_PRESS)
			m_Camera->MoveCamera(m_Camera->Backward, 2.5f, p_dt);

		if (glfwGetKey(p_window, GLFW_KEY_A) == GLFW_PRESS)
			m_Camera->MoveCamera(m_Camera->Left, 2.5f, p_dt);

		if (glfwGetKey(p_window, GLFW_KEY_D) == GLFW_PRESS)
			m_Camera->MoveCamera(m_Camera->Right, 2.5f, p_dt);
	}

    void Close() override
	{
		std::cout << "Scene Closed" << std::endl;
		ResourceManager::DeleteResources();
	}
};