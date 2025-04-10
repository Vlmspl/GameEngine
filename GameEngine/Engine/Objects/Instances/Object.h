#pragma once
#include "../../Renderer.h"
#include "../Instance.h"
#include "../Transform.h"
#include "../Material.h"

class Object : public Instance {
  public:
	Transform transform;
	GeometryContainer geometry;
    Material* material;

	Object(const std::string& name, Scene* scene, Material* material) : Instance(name, scene), material(material) {
		transform = Transform();
	}

     ~Object() = default;

	void OnCreation() override {};

	void SetMesh(const Mesh* mesh) {
		geometry.SetVertexData(*mesh);
	}

     void Render() override {
     	geometry.Bind();
		material->Use(this);

       	geometry.Draw();


     }
};