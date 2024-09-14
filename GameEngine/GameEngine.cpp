#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GLFW/glfw3.h"
#include <iostream>

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

        // Set up the fixed-function pipeline
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, 800, 0, 600); // Set up an orthographic projection matrix

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Draw a triangle
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); // Red vertex
        glVertex2f(100.0f, 100.0f);

        glColor3f(0.0f, 1.0f, 0.0f); // Green vertex
        glVertex2f(400.0f, 100.0f);

        glColor3f(0.0f, 0.0f, 1.0f); // Blue vertex
        glVertex2f(250.0f, 400.0f);
        glEnd();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
