#pragma once
#include <bitset>
#include <string>
#include <memory>
#include <iomanip>
#

class Scene;

static std::vector<int> idVector; // Vector storing the IDs (last used ID at the end)

class Instance {
protected:
    std::string id;     // Unique identifier (byte code-based)
    std::string name;   // Name of the instance
    Scene* scene;       // Scene reference

    // Generates a unique ID based on the existing table
    static std::string GenerateUniqueID() {
        // Generate a new ID by incrementing the last ID
        int newId = 0;

        if (!idVector.empty()) {
            newId = idVector.back() + 1;  // Increment the last ID in the vector
        }

        // Add the new ID to the vector
        idVector.push_back(newId);

        // Encode the ID to a string (we use a bitset for simplicity)
        std::bitset<64> bitsetId(newId);  // We can use 64 bits (or adjust as needed)
        std::string idString = bitsetId.to_string(); // Convert to binary string

        return idString; // Return the generated ID as a string
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
    static T* Create(Scene& targetScene, const std::string& instanceName, Args&&... args);
};