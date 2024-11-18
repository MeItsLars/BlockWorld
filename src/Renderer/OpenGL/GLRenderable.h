#pragma once
#include "../IRenderable.h"
#include "GL/glew.h"

class GLRenderable : public IRenderable {
public:
    GLRenderable();

    ~GLRenderable() override;

    size_t render(bool wireframe) override;

    void setMesh(const std::shared_ptr<Mesh> mesh) override {
        model.setMesh(mesh);
        if (mesh) initBuffers(mesh);
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

    GLuint vao = 0;
    GLuint vBuffer = 0;
    GLuint iBuffer = 0;

    void initBuffers(const std::shared_ptr<Mesh>& mesh) const;
};
