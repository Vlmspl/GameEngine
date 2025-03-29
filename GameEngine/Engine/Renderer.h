#pragma once
#include <glfw/glfw3.h>
#include <glad/glad.h>
#include "../Utilities.h"
#include "Device.h"
#include "Objects.h"
#include "Core.h"

class Renderer {
public:
  class BeforeRender : public Event {
  public:
    using Handler = std::function<void()>;  // Specialized handler with no parameters

    // Override Fire to call handlers without any parameters
    static void Fire() {
      for (const auto& handler : handlers) {
        if (handler) {
          (*handler)();  // Fire the handler with no arguments
        }
      }
    }

    // Implement OnEvent to add handlers and return a Connection
    static Connection OnEvent(Handler handler) {
      handlers.push_back(handler);  // Add the handler for BeforeRender
      return Connection(handlers.size() - 1);  // Return the connection object for disconnection
    }

  private:
    static std::vector<std::optional<Handler>> handlers;
  };

  class AfterRender : public Event {
  public:
    using Handler = std::function<void()>;  // Specialized handler with no parameters

    // Override Fire to call handlers without any parameters
    static void Fire() {
      for (const auto& handler : handlers) {
        if (handler) {
          (*handler)();  // Fire the handler with no arguments
        }
      }
    }

    // Implement OnEvent to add handlers and return a Connection
    static Connection OnEvent(Handler handler) {
      handlers.push_back(handler);  // Add the handler for AfterRender
      return Connection(handlers.size() - 1);  // Return the connection object for disconnection
    }

  private:
    static std::vector<std::optional<Handler>> handlers;
  };

public:
  static GLFWwindow* window;
  static VertexBuffer* vbo;
  static ElementBuffer* ebo;
  static VertexArray* vao;
  static Texture* texture;
  static ShaderProgram* shader;


  static void Setup(GLFWwindow* Window, VertexBuffer* Vbo, ElementBuffer* Ebo, VertexArray* Vao, Texture* Texture, ShaderProgram* Shader) {
    window = Window;
    vbo = Vbo;
    ebo = Ebo;
    vao = Vao;
    texture = Texture;
    shader = Shader;

  };

  static void Render(Camera* camera) {
    glfwPollEvents();
    Screen::Update(window);

    Time::Update();

    BeforeRender::Fire();

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
    AfterRender::Fire();
  }
};

GLFWwindow* Renderer::window = nullptr;
VertexBuffer* Renderer::vbo = nullptr;
ElementBuffer* Renderer::ebo = nullptr;
VertexArray* Renderer::vao = nullptr;
Texture* Renderer::texture = nullptr;
ShaderProgram* Renderer::shader = nullptr;

std::vector<std::optional<Renderer::BeforeRender::Handler>> Renderer::BeforeRender::handlers;
std::vector<std::optional<Renderer::AfterRender::Handler>> Renderer::AfterRender::handlers;