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
  static VertexBuffer* vbo;
  static ElementBuffer* ebo;
  static VertexArray* vao;
  static Texture* texture;
  static ShaderProgram* shader;

  static Event<> BeforeRender;
  static Event<> AfterRender;


  static void Setup(GLFWwindow* Window, VertexBuffer* Vbo, ElementBuffer* Ebo, VertexArray* Vao, Texture* Texture, ShaderProgram* Shader) {
    window = Window;
    vbo = Vbo;
    ebo = Ebo;
    vao = Vao;
    texture = Texture;
    shader = Shader;

  };

  static void Render(const Camera* camera) {
    glfwPollEvents();
    Screen::Update(window);

    Time::Update();

    BeforeRender.Fire();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, Screen::width, Screen::height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    shader->Use();
    Matrix4f view = camera->GetViewMatrix();
    shader->SetUniform("viewMatrix", view);
    Matrix4f projection = camera->GetProjectionMatrix();
    shader->SetUniform("projectionMatrix", projection);

    texture->bind();
    shader->SetUniform("Texture", 0);

    vao->Bind();
    vbo->Bind();
    ebo->Bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    AfterRender.Fire();
  }
};

GLFWwindow* Renderer::window = nullptr;
VertexBuffer* Renderer::vbo = nullptr;
ElementBuffer* Renderer::ebo = nullptr;
VertexArray* Renderer::vao = nullptr;
Texture* Renderer::texture = nullptr;
ShaderProgram* Renderer::shader = nullptr;