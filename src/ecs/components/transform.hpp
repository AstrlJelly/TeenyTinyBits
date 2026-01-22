#pragma once

#include <glm/glm/ext.hpp>

#include "ecs/component.hpp"

namespace teeny
{
    struct Transform : Component
    {
        glm::vec3 position;
        glm::quat rotation;
    };
}