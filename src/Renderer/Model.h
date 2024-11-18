#pragma once
#include "Mesh.h"
#include "Transform.h"

class Model {
public:
    void setMesh(const std::shared_ptr<Mesh> &mesh) {
        this->mesh = mesh;
    }

    std::shared_ptr<Mesh> getMesh() {
        return mesh;
    }

    Transform &getTransform() {
        return transform;
    }

private:
    // Pointer to mesh (multiple models can share the same mesh)
    std::shared_ptr<Mesh> mesh = nullptr;
    // Unique transform for each model, owned by the model
    Transform transform;
};
