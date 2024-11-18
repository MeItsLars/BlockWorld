#pragma once
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <utility>

#include "Texture.h"
#include "spdlog/spdlog.h"

enum class AttributeType {
    FLOAT,
    INT,
    UNSIGNED_INT
};

struct Attribute {
    std::string name;
    AttributeType type;
    int size;
    size_t stride;

    Attribute(std::string name, const AttributeType type, const int size, const size_t stride)
        : name(std::move(name)), type(type), size(size), stride(stride) {
    }
};

struct ShaderProgramInfo {
    std::string vertexSource;
    std::string fragmentSource;
    std::vector<Attribute> attributes;

    ShaderProgramInfo(std::string vertexSource, std::string fragmentSource, std::vector<Attribute> attributes)
        : vertexSource(std::move(vertexSource)), fragmentSource(std::move(fragmentSource)),
          attributes(std::move(attributes)) {
    }

    bool operator==(const ShaderProgramInfo &rhs) const {
        return vertexSource == rhs.vertexSource && fragmentSource == rhs.fragmentSource;
    }
};

namespace std {
    template<>
    struct hash<ShaderProgramInfo> {
        std::size_t operator()(const ShaderProgramInfo &source) const noexcept {
            return std::hash<std::string>()(source.vertexSource) ^ std::hash<std::string>()(source.fragmentSource);
        }
    };
}

class IShader {
public:
    virtual ~IShader() = default;

    virtual void load(const ShaderProgramInfo &source) = 0;

    virtual void use() = 0;

    virtual ShaderProgramInfo &getShaderProgramInfo() = 0;

    virtual void setTexture(const char *name, Texture *texture) = 0;

    virtual void setAttributes() = 0;

    virtual void setUniformMatrix3fv(const char *name, glm::mat3 matrix) = 0;

    virtual void setUniformMatrix4fv(const char *name, glm::mat4 matrix) = 0;

    static size_t getAttributeTypeSize(const AttributeType type) {
        switch (type) {
            case AttributeType::FLOAT:
                return sizeof(float);
            case AttributeType::INT:
                return sizeof(int);
            case AttributeType::UNSIGNED_INT:
                return sizeof(unsigned int);
            default:
                spdlog::error("Unknown attribute type. Defaulting to sizeof(float)");
                return sizeof(float);
        }
    }

    static std::string readShaderFile(const std::string &filePath) {
        std::string shaderSource;

        if (std::ifstream file(filePath); file.is_open()) {
            std::string line;
            while (getline(file, line)) {
                shaderSource.append(line + "\n");
            }
            file.close();
        } else {
            spdlog::error("Unable to open file: {}", filePath);
        }
        return shaderSource;
    }
};
