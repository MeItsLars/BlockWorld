#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

class ShaderProgram {
public:
    GLuint id;

    ShaderProgram() = default;

    ~ShaderProgram() = default;

    void initialize(std::string vShaderFile, std::string fShaderFile);

    void use() const;

    void debug() const;

    static void useNone() {
        glUseProgram(0);
    }
private:
    GLuint loadShader(std::string shaderFile, GLenum shaderType);

    std::string readShaderSource(std::string shaderFile);

    bool checkOpenGLError();
};
