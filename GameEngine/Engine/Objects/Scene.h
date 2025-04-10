#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include "Instance.h"

class Scene {
private:
	std::vector<std::unique_ptr<Instance>> instances;  // Stores all instances
	std::unordered_map<std::string, Instance*> instanceMap;  // Fast lookup by ID

public:
	Scene() = default;

	/**
	 * @brief Adds a new instance to the scene.
	 * @param instance A unique_ptr to an instance.
	 */
	void addInstance(std::unique_ptr<Instance> instance) {
		if (!instance) return;
		instance->OnCreation();
		std::string id = instance->getId();
		instanceMap[id] = instance.get();
		instances.push_back(std::move(instance));
	}

	/**
	 * @brief Removes an instance by its ID.
	 * @param id The ID of the instance to remove.
	 * @return True if removed, false if not found.
	 */
	bool removeInstance(const std::string& id) {
		auto it = instanceMap.find(id);
		if (it == instanceMap.end()) return false;

		// Remove from vector
		auto vecIt = std::find_if(instances.begin(), instances.end(),
								  [&](const std::unique_ptr<Instance>& inst) { return inst->getId() == id; });

		if (vecIt != instances.end()) instances.erase(vecIt);

		// Remove from map
		instanceMap.erase(it);
		return true;
	}

	/**
	 * @brief Finds an instance by ID.
	 * @param id The ID of the instance.
	 * @return Pointer to the instance, or nullptr if not found.
	 */
	Instance* findInstance(const std::string& id) {
		auto it = instanceMap.find(id);
		return (it != instanceMap.end()) ? it->second : nullptr;
	}

	/**
	 * @brief Gets all instances.
	 * @return A reference to the vector of instances.
	 */
	const std::vector<std::unique_ptr<Instance>>& getInstances() const {
		return instances;
	}
};

template<typename T, typename... Args>
T *Instance::Create(Scene &targetScene, const std::string &instanceName, Args &&... args) {
	auto instance = std::make_unique<T>(instanceName, &targetScene, std::forward<Args>(args)...);
	T* rawPtr = instance.get();  // Get raw pointer before moving ownership
	targetScene.addInstance(std::move(instance));
	return rawPtr;
}
