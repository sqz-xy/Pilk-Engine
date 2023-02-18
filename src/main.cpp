/* Resources
 * - https://learnopengl.com/Getting-started/Hello-Window
 */

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad.h"
#include "glfw3.h"
#include "stb_image.h"
#include "irrKlang.h"

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>

/**
 * \brief Prototype for viewport adjusting
 * \param window The current window
 * \param width width of the window
 * \param height height of the window
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/**
 * \brief Prototype for input processing
 * \param window The current window
 */
void processInput(GLFWwindow* window);

/**
 * \brief Loads a shader file
 * \param pFileName The name of the shader file to load
 * \param pShaderSource A reference The source code of the shader
 * \return A bool representing if the operation was successful
 */
bool LoadShader(const char* pFileName, std::string &pShaderSource)
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
bool compile_shader(const GLenum &pShaderType, const char* pFileName, unsigned int* pShaderBuffer, int* pSuccess, char* pInfoLog)
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
bool create_shader_program(unsigned int* pVertexShader, unsigned int* pFragmentShader, unsigned int* pShaderProgram)
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

/**
 * \brief The main program
 * \return exit code
 */
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
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
    glViewport(0, 0, 800, 600);

    // Resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    float colour[4] = { 1.0f, 0.5f, 0.2f, 1.0f };

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
    if (!create_shader_program(&vertexShader, &fragmentShader, &shaderProgram)) return -1;

    // Buffer Data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // ImGUI Init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Simple update loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ImGui frame init
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Input processing
        processInput(window);

        // Rendering commands here
        // ...

        glUseProgram(shaderProgram);

        // Modify shader uniforms
        glUniform4f(glGetUniformLocation(shaderProgram, "uColour"), colour[0], colour[1], colour[2], colour[3]);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // UI drawing
        ImGui::Begin("ImGui Test");
        ImGui::Text("Goo goo ga ga");
        ImGui::ColorEdit4("Colour", colour);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Clear Buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Delete Shaders
    glDeleteProgram(shaderProgram);

    // Delete all resources
    glfwTerminate();
    return 0;
}

/**
 * \brief Function for viewport adjusting
 * \param window The current window
 * \param width width of the window
 * \param height height of the window
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/**
 * \brief Function for input processing
 * \param window The current window
 */
void processInput(GLFWwindow* window)
{
    // Exit if escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}