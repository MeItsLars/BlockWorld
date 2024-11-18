#pragma once

#include "IShader.h"
#include "Model.h"

class IRenderable {
public:
    virtual ~IRenderable() = default;

    virtual size_t render(bool wireframe = false) = 0;

    virtual void setMesh(std::shared_ptr<Mesh> mesh) = 0;

    virtual Model &getModel() = 0;

    virtual void setShaderProgram(std::shared_ptr<IShader> shaderProgram) = 0;

    virtual std::shared_ptr<IShader> getShaderProgram() = 0;
};
