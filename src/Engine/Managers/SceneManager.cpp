#include "SceneManager.h"
#include "glad.h"
#include "glfw3.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <iostream>

SceneManager::SceneManager(const int p_width, const int p_height, char* p_windowName) : m_width(p_width), m_height(p_height), m_windowName(p_windowName)
{

}

SceneManager::SceneManager(const SceneManager& p_sceneManager) : m_width(p_sceneManager.m_width), m_height(p_sceneManager.m_height), m_windowName(p_sceneManager.m_windowName)
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
    return 0;
}

SceneManager& SceneManager::operator=(const SceneManager& p_rhs)
{
    if (&p_rhs != this)
    {
        m_width = p_rhs.m_width;
        m_height = p_rhs.m_height;
        m_windowName = p_rhs.m_windowName;
    }
    return *this;
}

void SceneManager::Render(const float p_dt) const
{

}

void SceneManager::Update(const float p_dt)
{

}

/**
* \brief Function for input processing
* \param window The current window
*/
void SceneManager::processInput(GLFWwindow* p_window)
{
    // Exit if escape key is pressed
    if (glfwGetKey(p_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(p_window, true);
}


