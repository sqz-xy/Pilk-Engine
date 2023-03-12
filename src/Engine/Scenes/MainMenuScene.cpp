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

	glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, 2.0f);

	glm::mat4 m_modelMat;
	
	
	Camera* m_Camera;


	explicit MainMenuScene(SceneManager* pSceneManager) : Scene(pSceneManager)
	{
		m_sceneManager->m_windowName = "MainMenuScene";
		
		m_Camera = new Camera(	glm::vec3(0.0f,0.0f,0.0f),	// camPos
								glm::vec3(0.0f,0.0f,-2.0f),  // camTarget
								glm::vec3(glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - glm::vec3(0.0f, 0.0f, -2.0f))), // cam direction 
								glm::vec3(0.0f,1.0f,0.0f),	// cam up
								m_sceneManager->m_width,	// windows width
								m_sceneManager->m_height);	// window height


		// rotation
		// scale
		//m_modelMat = glm::rotate(m_modelMat, glm::radians(21.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		m_modelMat = glm::translate(glm::mat4(1.0f), modelPos); // translatng the model in the matrix
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
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
		float vertices[] = {
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

		unsigned int indices[] = {
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

		unsigned int VBO, VAO, EBO;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Creates a shader program
		unsigned int shaderProgram;
		if (!ResourceManager::CreateShaderProgram(&shaderProgram, "resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag")) return;

		
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