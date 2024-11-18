#include "GLRenderable.h"

GLRenderable::GLRenderable() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vBuffer);
    glGenBuffers(1, &iBuffer);
    ObjectChecker::addRenderable();
}

GLRenderable::~GLRenderable() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vBuffer);
    glDeleteBuffers(1, &iBuffer);
    ObjectChecker::removeRenderable();
}

void GLRenderable::initBuffers(const std::shared_ptr<Mesh>& mesh) const {
    if (shaderProgram == nullptr) {
        spdlog::error("Shader program is not set");
        return;
    }
    if (mesh == nullptr) {
        spdlog::error("Mesh is not set");
        return;
    }
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizei>(mesh->getTotalVerticesSize()),
        mesh->getVerticesData(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizei>(mesh->getTotalFacesSize()),
        mesh->getFacesData(),
        GL_STATIC_DRAW
    );

    shaderProgram->setAttributes();
}

size_t GLRenderable::render(bool wireframe) {
    glBindVertexArray(vao);

    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(model.getMesh()->getFacesAttributeCount()),
        GL_UNSIGNED_INT,
        nullptr
    );

    return model.getMesh()->getVertexCount();
}
