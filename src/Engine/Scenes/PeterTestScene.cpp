#pragma once

#include "Scene.h"

#include "imgui_impl_opengl3.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "iostream"
#include "ostream"
#include "string"

#include "../Managers/ResourceManager.h"
#include "../Utility/Camera.h"	
#include "../Components/Component.h"
#include "../Systems/System.h"
#include "../Objects/Entity.h"


class PeterTestScene : public Scene
{
public:

	// TODO: Remove after refactor
	unsigned int m_vao;
	unsigned int m_shaderProgramID;
	float* m_colour = new float[4] { 1.0f, 0.5f, 0.2f, 1.0f };

	glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 modelPos2 = glm::vec3(0.6f, 0.0f, 2.0f);

	glm::mat4 m_modelMat;
	glm::mat4 m_modelMat2;

	Camera* m_Camera;

	ComponentTransform* m_transformation = new ComponentTransform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));


	explicit PeterTestScene(SceneManager* pSceneManager) : Scene(pSceneManager)
	{
		m_sceneManager->m_windowName = "PeterTestScene";

		glm::vec3 camPos = glm::vec3(0.0f, 0.0f, -2.0f);
		glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, 0.0f);

		m_Camera = new Camera(
			camPos,	// camPos
			camTarget,  // camTarget
			glm::vec3(glm::normalize(camPos - camTarget)), // cam direction 
			glm::vec3(0.0f, 1.0f, 0.0f),	// cam up
			m_sceneManager->m_width,	// windows width
			m_sceneManager->m_height);	// window height


		m_modelMat = glm::translate(glm::mat4(1.0f), modelPos); // translatng the model in the matrix
		m_modelMat2 = glm::translate(glm::mat4(1.0f), modelPos2);

		TestConstructor();
	}

	~PeterTestScene() override
	{
		this->Close();
	}



	// -------- Test Area --------

	SystemPhysics* m_testSystemPhysics = new SystemPhysics();
	Entity* m_testEntity;

	
	// By: Piotr Moskala
	void TestConstructor()
	{
		// Create Entity
		m_testEntity = new Entity("Test Entity");
		m_testEntity->AddComponent(m_transformation);

		// Check GetComponent method works
		ComponentTransform* component = m_testEntity->GetComponent<ComponentTransform>();

		// Validate Entity against System
		m_testSystemPhysics->ValidateEntity(m_testEntity);

		// Check if RemoveComponent & removing invalid Entity from system works
		/*m_testEntity->RemoveComponent(component);
		m_testSystemPhysics->ValidateEntity(m_testEntity);*/
	}

	// By: Piotr Moskala
	void TestUpdate(const float p_dt)
	{
		// System physics update checks
		m_testSystemPhysics->Execute(p_dt);
		
		// Updated transform
		ComponentTransform* component = m_testEntity->GetComponent<ComponentTransform>();

		// Checking if component update works outside of system
		/*vec3 rot = component->m_rotation;
		component->UpdateRotation(glm::vec3(rot.x, rot.y, rot.z + 0.5 * p_dt));*/
	}

	// ---------------------------


	void Render(const float p_dt) const override
	{
		glUseProgram(m_shaderProgramID);

		// Change Colour
		glUniform4f(glGetUniformLocation(m_shaderProgramID, "uColour"), m_colour[0], m_colour[1], m_colour[2], m_colour[3]);

		m_Camera->UpdateCamera(m_shaderProgramID);

		glBindVertexArray(m_vao);

		// Draw two cubes
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, "uModel"), 1, GL_FALSE, &m_transformation->m_transform[0][0]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, "uModel"), 1, GL_FALSE, &m_modelMat2[0][0]);
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		ImVec2 vec(300, 300);
		
		//std::cout << "Rendering" << std::endl;
		ImGui::Begin("ImGui Test");
		ImGui::SetWindowSize(vec);
		ImGui::Text("Delta time %f", p_dt);
		ImGui::ColorEdit4("Colour", m_colour);
		

		glm::vec3 translation = m_testEntity->GetComponent<ComponentTransform>()->m_translation;
		ImGui::Text("componentXYZ %f %f %f", translation.x, translation.y, translation.z);

		glm::vec3 rotation = m_testEntity->GetComponent<ComponentTransform>()->m_rotation;
		ImGui::Text("componentXYZ %f %f %f", rotation.x, rotation.y, rotation.z);

		bool button = ImGui::Button("Main Menu", vec);

		if (button)
			m_sceneManager->ChangeScene(MainMenu);



		glUseProgram(0);

		ImGui::End();
	}

	void Update(const float p_dt) override
	{
		glUseProgram(m_shaderProgramID);
		m_Camera->UpdateCamera(m_shaderProgramID);


		TestUpdate(p_dt);
		
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

	void ProcessInput(GLFWwindow* p_window, const float p_dt, bool p_mouseInput) override
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

		if (p_mouseInput)
		{
			double xpos, ypos;
			glfwGetCursorPos(p_window, &xpos, &ypos);
			m_Camera->RotateCamera(glm::vec2(xpos, ypos));
		}
	}

	void Close() override
	{
		std::cout << "Scene Closed" << std::endl;
		ResourceManager::DeleteResources();
	}
};