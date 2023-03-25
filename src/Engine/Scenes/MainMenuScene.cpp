#pragma once

#include "Scene.h"
#include "../Managers/ResourceManager.h"
#include "../Utility/Camera.h"	
#include "../PilkEngineCommon.h"
#include "../Objects/Geometry.h"
#include "../Objects/Model.h"
#include "../Components/Component.h"


class MainMenuScene : public Scene
{
public:

	ComponentTransform* m_transformation = new  ComponentTransform(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// TODO: Remove after refactor
	unsigned int m_shaderProgramID;
	float* m_colour = new float[4] { 1.0f, 0.5f, 0.2f, 1.0f };

	glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 modelPos2 = glm::vec3(0.6f, 0.0f, 2.0f);

	glm::mat4 m_modelMat;
	glm::mat4 m_modelMat2;
	
	Camera* m_Camera;

	Model* m_backpack;
	Model* m_backpack2;

	ComponentGeometry* m_geometry;

	explicit MainMenuScene(SceneManager* pSceneManager) : Scene(pSceneManager)
	{
		m_sceneManager->m_windowName = "MainMenuScene";
		
		m_Camera = new Camera(	glm::vec3(0.0f,0.0f,-5.0f),	// camPos
								glm::vec3(0.0f,0.0f,5.0f),  // camTarget
								m_sceneManager->m_width,	// windows width
								m_sceneManager->m_height);	// window height


		// rotation
		// scale
		//m_modelMat = glm::rotate(m_modelMat, glm::radians(21.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		m_modelMat = glm::translate(glm::mat4(1.0f), modelPos); // translatng the model in the matrix

		m_modelMat = m_transformation->m_transform;

		m_modelMat2 = glm::translate(glm::mat4(1.0f), modelPos2);
		//m_modelMat = glm::rotate(m_modelMat, 72.f, glm::vec3(1, 1, 1));
	}


	~MainMenuScene() override
	{
		this->Close();
	}

	void Render(const float p_dt) const override
	{
		glUseProgram(m_shaderProgramID);

		// Change Colour
		glUniform4f(glGetUniformLocation(m_shaderProgramID, "uColour"), m_colour[0], m_colour[1], m_colour[2], m_colour[3]);
		
		m_Camera->UpdateCamera(m_shaderProgramID);

		// Geometry component
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, "uModel"), 1, GL_FALSE, &m_modelMat[0][0]);
		m_geometry->Draw(m_shaderProgramID);

		//glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, "uModel"), 1, GL_FALSE, &m_modelMat2[0][0]);
		//m_backpack->Draw(m_shaderProgramID);

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
		glUseProgram(m_shaderProgramID);
		m_Camera->UpdateCamera(m_shaderProgramID);
	}

	void Load() override
	{
		stbi_set_flip_vertically_on_load(true);

		m_backpack = ResourceManager::LoadModel("resources/models/backpack/backpack.obj");
		m_backpack2 = ResourceManager::LoadModel("resources/models/backpack/backpack.obj");

		m_geometry = new ComponentGeometry("resources/models/backpack/backpack.obj");

		if (!ResourceManager::CreateShaderProgram(&m_shaderProgramID, "resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag")) return;
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

#if MOUSE_CONTROL_ENABLED
		double xpos, ypos;
		glfwGetCursorPos(p_window, &xpos, &ypos);
		m_Camera->RotateCamera(glm::vec2(xpos, ypos));
#endif		
	}

    void Close() override
	{
		std::cout << "Scene Closed" << std::endl;

		delete m_Camera;
		delete[] m_colour;

		ResourceManager::DeleteResources();
	}
};