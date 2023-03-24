#pragma once

#include "SceneManager.h"

#include "../Scenes/MainMenuScene.cpp"
#include "../PilkEngineCommon.h"

SceneManager::SceneManager(const int p_width, const int p_height, char* p_windowName) : m_currentScene(nullptr), m_width(p_width), m_height(p_height), m_windowName(p_windowName)
{
    m_currentScene = new MainMenuScene(this);
}

SceneManager::SceneManager(const SceneManager& p_sceneManager) : m_currentScene(p_sceneManager.m_currentScene), m_width(p_sceneManager.m_width), m_height(p_sceneManager.m_height), m_windowName(p_sceneManager.m_windowName)
{

}

SceneManager::~SceneManager()
{
    delete m_currentScene;
    m_windowName = nullptr;
    m_currentScene = nullptr;
}

// By Thomas Beet
/// <summary>
/// Runs the program mainloop
/// </summary>
/// <returns>exit code</returns>
int SceneManager::Run() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(m_width, m_height, m_windowName, nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Set the window as the main context of the current thread
    glfwMakeContextCurrent(window);

#if MOUSE_CONTROL_ENABLED
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif

    // Initialize GLAD as it manages opengl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set viewport dimensions
    glViewport(0, 0, m_width, m_height);

    glfwSetWindowUserPointer(window, this);
    // Resize callback   
    // ImGUI Init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glEnable(GL_DEPTH_TEST);

    SceneManager::Load();

    float dt;
    auto lastTime = static_cast<float>(glfwGetTime());

    // Simple update loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_DEPTH_BUFFER_BIT);

        auto now = static_cast<float>(glfwGetTime());
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
        processInput(window, dt);

        // Current scene render/update
        SceneManager::Render(dt);

        SceneManager::Update(dt);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Delete all resources
    glfwTerminate();

    SceneManager::~SceneManager();
    return 0;
}

// By Thomas Beet
/// <summary>
/// Changes the current scene, deletes the previous
/// </summary>
/// <param name="p_sceneType">Scene type</param>
void SceneManager::ChangeScene(const SceneTypes p_sceneType)
{
    switch (p_sceneType)
    {
    case MainMenu:
        delete m_currentScene;
        m_currentScene = new MainMenuScene(this);
        break;
    default:
        delete m_currentScene;
        m_currentScene = new MainMenuScene(this);
        break;
    }

    std::cout << "Scene Changed" << std::endl;
    SceneManager::Load();
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

// By Thomas Beet
/// <summary>
/// Load logic for current scene
/// </summary>
void SceneManager::Load()
{
    m_currentScene->Load();
}

// By Thomas Beet
/// <summary>
/// Render logic for current scene
/// </summary>
/// <param name="p_dt">delta time</param>
void SceneManager::Render(const float p_dt) const
{
    m_currentScene->Render(p_dt);
}


/// <summary>
/// Update logic for current scene
/// </summary>
/// <param name="p_dt">delta time</param>
void SceneManager::Update(const float p_dt)
{
    m_currentScene->Update(p_dt);
}


/// <summary>
/// Input processing
/// </summary>
/// <param name="p_window">The current window</param>
void SceneManager::processInput(GLFWwindow* p_window, const float p_dt)
{
    m_currentScene->ProcessInput(p_window, p_dt);
}
