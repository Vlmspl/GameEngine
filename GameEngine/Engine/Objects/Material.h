#pragma once
#include <any>
#include <vector>
#include <string>
#include <functional>
#include "../Core.h"

class Object; // Forward declaration for Object class

class UniformValue {
public:
    std::string name;
    std::function<std::any(Object*, ShaderProgram*)> getValue;

    // Constructor with name and getter function that takes Object and ShaderProgram pointers
    UniformValue(const std::string& uniformName, std::function<std::any(Object*, ShaderProgram*)> uniformGetter)
        : name(uniformName), getValue(uniformGetter) {}

    // Specialization for directly using a specific type (e.g., Matrix4f)
    template<typename T>
    UniformValue(const std::string& uniformName, std::function<T(Object*, ShaderProgram*)> uniformGetter)
        : name(uniformName), getValue([uniformGetter](Object* obj, ShaderProgram* shader) {
            return std::any(uniformGetter(obj, shader));  // Wrap the return value in std::any
        }) {}
};

class Material {
public:
    std::vector<UniformValue> uniforms;
    ShaderProgram* shader;

    // Constructor that allows adding uniforms directly to the Material
    template<typename... Args>
    Material(ShaderProgram* program, Args&&... args) {
        shader = program;
        (AddUniform(std::forward<Args>(args)), ...); // Renamed SetUniform to AddUniform
    }

    // Function to add a uniform value to the list
    void AddUniform(const UniformValue& uniform) {
        uniforms.push_back(uniform);
    }

    // Use function that sets all uniforms
    void Use(Object* obj) {
        shader->Use();
        for (const auto& uniform : uniforms) {
            shader->NoTypeSetUniform(uniform.name, uniform.getValue(obj, shader));
        }
    }

    static void UnBind() {
      ShaderProgram::UnBind();
      Texture::UnBind();
    }
};
