#pragma once
#include "../IShader.h"

class EShader : public IShader {
public:
    EShader() {}

    ~EShader() override {}

    void load(const ShaderProgramInfo &shaderInfo) override {
        this->shaderInfo = shaderInfo;
    }

    ShaderProgramInfo &getShaderProgramInfo() override {
        return shaderInfo;
    }

    void use() override {}

    void setTexture(const char *name, Texture *texture) override {}

    void setAttributes() override {}

    void setUniformMatrix3fv(const char *name, glm::mat3 matrix) override {}

    void setUniformMatrix4fv(const char *name, glm::mat4 matrix) override {}

private:
    ShaderProgramInfo shaderInfo = ShaderProgramInfo("", "", {});
};
