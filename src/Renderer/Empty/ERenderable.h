#pragma once
#include "../IRenderable.h"

class ERenderable : public IRenderable {
public:
    ERenderable() {}

    ~ERenderable() override {}

    size_t render(bool wireframe) override { return 0; }

    void setMesh(const std::shared_ptr<Mesh> mesh) override {
        model.setMesh(mesh);
    }

    Model &getModel() override {
        return model;
    }

    void setShaderProgram(const std::shared_ptr<IShader> shaderProgram) override {
        this->shaderProgram = shaderProgram;
    }

    std::shared_ptr<IShader> getShaderProgram() override {
        return shaderProgram;
    }

private:
    Model model;
    std::shared_ptr<IShader> shaderProgram = nullptr;
};
