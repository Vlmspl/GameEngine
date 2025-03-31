#include "../../Renderer.h"
#include "../Instance.h"
#include "../Transform.h"

class Object : public Instance {
  public:
    Transform transform;
    Mesh mesh;

     Object() = default;
     ~Object() = default;

     void Render() override {
       mesh.Bind();

     }
};