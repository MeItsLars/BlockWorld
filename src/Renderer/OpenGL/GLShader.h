#pragma once
#include "../IShader.h"
#include "GL/glew.h"

class GLShader : public IShader {
public:
    GLShader() = default;

    ~GLShader() override;

    void load(const ShaderProgramInfo &shaderInfo) override;

    ShaderProgramInfo &getShaderProgramInfo() override {
        return shaderInfo;
    }

    void use() override;

    void setTexture(const char *name, Texture *texture) override;

    void setAttributes() override;

    void setUniformMatrix3fv(const char *name, glm::mat3 matrix) override;

    void setUniformMatrix4fv(const char *name, glm::mat4 matrix) override;

private:
    ShaderProgramInfo shaderInfo = ShaderProgramInfo("", "", {});
    GLuint programId = 0;
    GLuint textureId = 0;
    std::unordered_map<std::string, GLint> attributeLocationCache;
    std::unordered_map<std::string, GLint> uniformLocationCache;

    bool checkOpenGLError();

    static GLenum getGLType(AttributeType type);

    GLint getAttributeLocation(const char *name);

    GLint getUniformLocation(const char *name);

    GLuint compileShader(GLenum type, const std::string &source);
};
