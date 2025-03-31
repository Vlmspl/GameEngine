#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <bits/random.h>

class Scene;
class Instance {
protected:
    std::string id;     // Unique identifier (byte code-based)
    std::string name;   // Name of the instance
    Scene* scene;       // Scene reference

    // Generates a random byte-code-like unique ID
    static std::string GenerateUniqueID() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 255);

        std::string uid;
        for (int i = 0; i < 8; i++) {  // 8-byte random ID
            uid += static_cast<char>(dist(gen));
        }
        return uid;
    }

public:
    Instance(const std::string& instanceName, Scene* parentScene)
        : name(instanceName), scene(parentScene), id(GenerateUniqueID()) {}

    virtual ~Instance() = default;

    virtual void OnCreation() = 0;  // Must be overridden
    virtual void Render() = 0;

    // Getters
    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    Scene* GetScene() const { return scene; }

    // Static creation method (One-line instance creation)
    template <typename T, typename... Args>
    static T* Create(Scene& targetScene, const std::string& instanceName, Args&&... args) {
        auto instance = std::make_unique<T>(instanceName, &targetScene, std::forward<Args>(args)...);
        T* rawPtr = instance.get();  // Get raw pointer before moving ownership
        targetScene.AddInstance(std::move(instance));
        return rawPtr;
    }
};