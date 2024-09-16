#include <windows.h>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Utils.h"
#include "Camera.h"
#include "Object.h"


int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24); // Request a 24-bit depth buffer

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        return -1;
    }
    

    // Make the window's context current
    glfwMakeContextCurrent(window);
    gladLoadGL();

    static const Vertex vertices[] = {
        { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },  // Bottom-left
        { {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },  // Bottom-right
        { {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f} },  // Top-right
        { {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} }   // Top-left
    };

    GLuint indices[] = {
        0, 1, 2,  // First triangle: bottom-left, bottom-right, top-right
        0, 2, 3   // Second triangle: bottom-left, top-right, top-left
    };




    std::string vertexCodeStr = ReadShaderCode("Assets/Shaders/TestShader.vert");
    std::string fragmentCodeStr = ReadShaderCode("Assets/Shaders/TestShader.frag");

    const char* VertexCode = vertexCodeStr.c_str();
    const char* FragmentCode = fragmentCodeStr.c_str();

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &VertexCode, NULL);
    glCompileShader(vertex_shader);
    checkShaderCompileErrors(vertex_shader, "VERTEX");

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &FragmentCode, NULL);
    glCompileShader(fragment_shader);
    checkShaderCompileErrors(fragment_shader, "FRAGMENT");

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    const GLint utime_location = glGetUniformLocation(program, "u_time");
    const GLint uprojection_location = glGetUniformLocation(program, "u_projection");
    const GLint uview_location = glGetUniformLocation(program, "u_view");
    const GLint utexture_location = glGetUniformLocation(program, "u_texture");

    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vuv_location = glGetAttribLocation(program, "vUv");

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(vuv_location);
    glVertexAttribPointer(vuv_location, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, uv));

    GLuint element_buffer;
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    const float fov = glm::radians(70.0f); // Field of view in radians
    const float nearPlane = 0.1f; // Near clipping plane
    const float farPlane = 10000.0f; // Far clipping plane

    // Main loop

    //glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back faces
    glFrontFace(GL_CCW); // Define counter-clockwise as front

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE); // Ensure depth writes are enabled (default)

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


    Camera camera = Camera();

    float lastFrame = 0;
    GLuint texture = loadTexture("Assets/Textures/texture.png", GL_TEXTURE0, GL_NEAREST, GL_REPEAT);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        std::cout << "\nFPS: " << 1.0f / deltaTime;

        // Poll for and process events
        glfwPollEvents();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height); // Aspect ratio (width/height)

        //Update Stuff:
        glm::vec3 Velocity = GetKeyboardMovement(window, 1);

        glm::vec3 CurrentRotation = camera.GetRotation();
        glm::vec3 NewRotation = CalculateNewRotation(window, CurrentRotation, 0.25);

        camera.setDeltaTime(deltaTime);

        camera.setMoveSpeed(Velocity);
        camera.SetRotation(NewRotation);

        cursor_position_callback(window, (double)width / 2, (double)height / 2);

        glm::mat4 Perspective = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
        glm::mat4 View = camera.GetViewMatrix();

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0.5, 0.5, 1.0);

        glUseProgram(program);
        glUniform1f(utime_location, static_cast<float>(glfwGetTime()));


        glUniformMatrix4fv(uprojection_location, 1, GL_FALSE, glm::value_ptr(Perspective));
        glUniformMatrix4fv(uview_location, 1, GL_FALSE, glm::value_ptr(View));

        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(utexture_location, 0);

        glBindVertexArray(vertex_array);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap front and back buffers
        glfwSwapBuffers(window);

    }
    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
