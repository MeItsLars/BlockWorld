#pragma once

#include <memory>

#include "../Renderer/Camera.h"
#include "../Renderer/IRenderable.h"

class Entity {
public:
    Entity();

    ~Entity();

    size_t render(const Camera *camera) const;

private:
    std::unique_ptr<IRenderable> renderable;
};
