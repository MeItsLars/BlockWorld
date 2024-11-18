#include "ShaderProgram.h"

bool ShaderProgram::checkOpenGLError() {
    bool foundError = false;
    GLenum glError = glGetError();

    while (glError != GL_NO_ERROR) {
        std::cerr << "glError: " << glError << std::endl;
        foundError = true;
        glError = glGetError();
    }
    return foundError;
}

std::string ShaderProgram::readShaderSource(const std::string shaderFile) {
    std::string shaderSource;
    std::string line;

    std::fstream fs(shaderFile, std::ios::in);
    if (!fs)
        return shaderSource;

    while (!fs.eof()) {
        getline(fs, line);
        shaderSource.append(line + '\n');
    }
    fs.close();
    return shaderSource;
}

GLuint ShaderProgram::loadShader(const std::string shaderFile, GLenum shaderType) {
    // Read shader file
    std::string shaderSource = readShaderSource(shaderFile);
    if (shaderSource.empty()) {
        std::cerr << "Failed to read " << shaderFile << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create shader object
    GLuint shader = glCreateShader(shaderType);
    const char *shaderSrc = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);
    checkOpenGLError();

    // Check compilation status
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint logSize;

        std::cerr << "Failed to compile " << shaderFile << std::endl;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
        if (logSize > 0) {
            char logMsg[logSize + 1];
            glGetShaderInfoLog(shader, logSize, nullptr, &(logMsg[0]));
            std::cerr << "Shader info log: " << logMsg << std::endl;
        }
        exit(EXIT_FAILURE);
    }

    // Return shader object
    return shader;
}

void ShaderProgram::initialize(const std::string vShaderFile, const std::string fShaderFile) {
    // Create program
    id = glCreateProgram();

    // Load shaders and attach them to program
    glAttachShader(id, loadShader(vShaderFile, GL_VERTEX_SHADER));
    glAttachShader(id, loadShader(fShaderFile, GL_FRAGMENT_SHADER));

    // Link program
    glLinkProgram(id);
    checkOpenGLError();

    GLint linked;
    glGetProgramiv(id, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint logSize;

        std::cerr << "Shader program failed to link!" << std::endl;

        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logSize);
        if (logSize > 0) {
            char logMsg[logSize + 1];
            glGetProgramInfoLog(id, logSize, NULL, &(logMsg[0]));
            std::cerr << "Program info log: " << logMsg << std::endl;
        }
        exit(EXIT_FAILURE);
    }
}

void ShaderProgram::use() const {
    glUseProgram(id);
}

void ShaderProgram::debug() const {
    GLint logSize;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize > 0) {
        std::cerr << "Failure in shader " << std::endl;
        char logMsg[logSize + 1];
        glGetProgramInfoLog(id, logSize, nullptr, &(logMsg[0]));
        std::cerr << "Shader info log: " << logMsg << std::endl;
    }
}