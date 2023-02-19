#include "SceneManager.h"
#include "glad.h"
#include "glfw3.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <iostream>

SceneManager::SceneManager(const int pWidth, const int pHeight, char* pWindowName) : m_width(pWidth), m_height(pHeight), m_windowName(pWindowName)
{

}

SceneManager::SceneManager(const SceneManager& pSceneManager) : m_width(pSceneManager.m_width), m_height(pSceneManager.m_height), m_windowName(pSceneManager.m_windowName)
{

}

SceneManager::~SceneManager(void)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Delete all resources
    glfwTerminate();

    m_windowName = nullptr;
}

void SceneManager::Load()
{

}

int SceneManager::Run() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(m_width, m_height, m_windowName, NULL, NULL);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Set the window as the main context of the current thread
    glfwMakeContextCurrent(window);

    // Initialize GLAD as it manages opengl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set viewport dimensions
    glViewport(0, 0, m_width, m_height);

    // Resize callback
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ImGUI Init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    SceneManager::Load();

    float dt;
    float lastTime = glfwGetTime();

    // Simple update loop
    while (!glfwWindowShouldClose(window))
    {
        float now = glfwGetTime();
        dt = now - lastTime;
        lastTime = now;
        
        // Clear colour
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ImGui frame init
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Input processing
        processInput(window);

        SceneManager::Render(dt);

        SceneManager::Update(dt);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void SceneManager::Render(const float pDt) const
{

}

void SceneManager::Update(const float pDt)
{

}

/**
* \brief Function for input processing
* \param window The current window
*/
void SceneManager::processInput(GLFWwindow* window)
{
    // Exit if escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


