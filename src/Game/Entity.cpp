#include "Entity.h"

#include "../Renderer/IShader.h"
#include "../Renderer/RendererProvider.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/ResourcePaths.h"
#include "spdlog/spdlog.h"

Entity::Entity() {
    spdlog::info("Spawning new entity");
    renderable = RendererProvider::createRenderable();
    renderable->setShaderProgram(ResourceManager::getOrLoadShader(Shaders::DEFAULT));
    renderable->setMesh(ResourceManager::getOrLoadMesh(ResourcePaths::ENTITY_MESH));
    renderable->getModel().getTransform().translate(0, -3, 0);
}

Entity::~Entity() {
    spdlog::info("Destroying entity");
    renderable.reset();
}

size_t Entity::render(const Camera *camera) const {
    // Step 1: Use the shader program associated with this entity
    const auto shader = renderable->getShaderProgram();
    shader->use();

    // Step 2: Update the specific shader program variables
    shader->setUniformMatrix4fv("P", camera->projectionMatrix);
    shader->setUniformMatrix4fv("V", camera->viewMatrix);
    shader->setUniformMatrix4fv("M", renderable->getModel().getTransform().getModelMatrix());
    shader->setUniformMatrix3fv("N", renderable->getModel().getTransform().getModelMatrix());

    // Step 3: Render the entity
    return renderable->render(true);
}
