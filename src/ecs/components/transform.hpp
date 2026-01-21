#pragma once

#include <glm/glm/ext.hpp>

#include "ecs/component.hpp"

struct Transform : Component
{
    glm::vec3 position;
    glm::quat rotation;
};