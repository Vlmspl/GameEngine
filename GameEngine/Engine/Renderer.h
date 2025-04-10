#pragma once
#include <glfw/glfw3.h>
#include <glad/glad.h>
#include "../Utilities.h"
#include "Device.h"
#include "Objects.h"
#include "Core.h"

class Renderer {
public:
  static GLFWwindow* window;

  static Event<> BeforeRender;
  static Event<> AfterRender;


  static void Setup(GLFWwindow* Window) {
    window = Window;
  };

  static void Render(Scene* scene, const Camera* camera) {
    glfwPollEvents();
    Screen::Update(window);
    Time::Update();

    BeforeRender.Fire();

    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glViewport(0, 0, Screen::width, Screen::height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto& instances = scene->getInstances();

    for (const auto& instance : instances) {
      instance->Render();
    }

    glfwSwapBuffers(window);
    AfterRender.Fire();
  }
};

GLFWwindow* Renderer::window = nullptr;

Event<> Renderer::BeforeRender;
Event<> Renderer::AfterRender;