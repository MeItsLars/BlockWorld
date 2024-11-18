#include "GLShader.h"

#include "../Mesh.h"

bool GLShader::checkOpenGLError() {
    bool foundError = false;
    GLenum glError = glGetError();

    while (glError != GL_NO_ERROR) {
        spdlog::error("OpenGL error: {}", glError);
        foundError = true;
        glError = glGetError();
    }
    return foundError;
}

GLenum GLShader::getGLType(const AttributeType type) {
    switch (type) {
        case AttributeType::FLOAT:
            return GL_FLOAT;
        case AttributeType::INT:
            return GL_INT;
        case AttributeType::UNSIGNED_INT:
            return GL_UNSIGNED_INT;
        default:
            spdlog::error("Unknown attribute type. Defaulting to GL_FLOAT");
            return GL_FLOAT;
    }
}

GLint GLShader::getAttributeLocation(const char *name) {
    // Check if the attribute location is already cached
    if (attributeLocationCache.find(name) != attributeLocationCache.end()) {
        return attributeLocationCache[name];
    }

    // Get the attribute location
    GLint location = glGetAttribLocation(programId, name);
    if (location == -1) {
        spdlog::error("Attribute '{}' not found. Is it optimized out?", name);
    }

    // Cache and return the attribute location
    attributeLocationCache[name] = location;
    return location;
}


GLint GLShader::getUniformLocation(const char *name) {
    // Check if the uniform location is already cached
    if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
        return uniformLocationCache[name];
    }

    // Get the uniform location
    GLint location = glGetUniformLocation(programId, name);
    if (location == -1) {
        spdlog::error("Uniform '{}' not found. Is it optimized out?", name);
    }

    // Cache and return the uniform location
    uniformLocationCache[name] = location;
    return location;
}

GLuint GLShader::compileShader(GLenum type, const std::string &source) {
    std::string shaderSource = readShaderFile(source);
    if (shaderSource.empty()) {
        spdlog::error("Failed to read shader file: {}", source);
        return 0;
    }

    // Create shader
    GLuint shader = glCreateShader(type);
    const GLchar *sourceCStr = shaderSource.c_str();
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);
    checkOpenGLError();

    // Check if the shader is compiled
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint logSize = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

        spdlog::error("Failed to compile shader {}", source);
        if (logSize > 0) {
            std::vector<GLchar> errorLog(logSize);
            glGetShaderInfoLog(shader, logSize, &logSize, &errorLog[0]);
            spdlog::error("{}", errorLog.data());
        }
        glDeleteShader(shader);
        return 0;
    }

    // Return the compiled shader
    return shader;
}


void GLShader::load(const ShaderProgramInfo &shaderInfo) {
    spdlog::info(R"([load] shader: "{}", "{}")", shaderInfo.vertexSource, shaderInfo.fragmentSource);
    programId = glCreateProgram();

    // Compile shaders
    const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, shaderInfo.vertexSource);
    glAttachShader(programId, vertexShader);
    glDeleteShader(vertexShader);
    const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, shaderInfo.fragmentSource);
    glAttachShader(programId, fragmentShader);
    glDeleteShader(fragmentShader);

    // Link shaders
    glLinkProgram(programId);
    glValidateProgram(programId);
    checkOpenGLError();

    // Check if the shaders are linked
    GLint isLinked = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint logSize = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);

        spdlog::error("Failed to link shader program");
        if (logSize > 0) {
            std::vector<GLchar> errorLog(logSize);
            glGetProgramInfoLog(programId, logSize, &logSize, &errorLog[0]);
            spdlog::error("{}", errorLog.data());
        }
    }
    this->shaderInfo = shaderInfo;
}

GLShader::GLShader() {
    ObjectChecker::addShader();
}

GLShader::~GLShader() {
    if (textureId != 0) {
        glDeleteTextures(1, &textureId);
    }
    glDeleteProgram(programId);
    ObjectChecker::removeShader();
}

void GLShader::use() {
    glUseProgram(programId);
}

void GLShader::setTexture(const char *name, Texture *texture) {
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(getUniformLocation(name), 0);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLint format = texture->nrChannels == 3 ? GL_RGB : GL_RGBA;

    if (unsigned char *data = texture->getData()) {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            texture->width,
            texture->height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        spdlog::info("Set OpenGL shader texture: {} (id {})", name, textureId);
    } else {
        spdlog::error("Failed to set OpenGL texture, data is null");
    }
}

void GLShader::setAttributes() {
    // Loop through the attributes in the shader program
    unsigned long long offset = 0;
    for (const auto &attribute: shaderInfo.attributes) {
        const GLint location = getAttributeLocation(attribute.name.c_str());
        glVertexAttribPointer(
            location,
            attribute.size,
            getGLType(attribute.type),
            GL_FALSE,
            static_cast<GLsizei>(attribute.stride),
            reinterpret_cast<void *>(offset)
        );
        glEnableVertexAttribArray(location);
        offset += attribute.size * getAttributeTypeSize(attribute.type);
    }
}


void GLShader::setUniformMatrix3fv(const char *name, glm::mat3 matrix) {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void GLShader::setUniformMatrix4fv(const char *name, glm::mat4 matrix) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}
