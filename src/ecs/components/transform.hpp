#pragma once

#include "glm/glm/glm.hpp" // IWYU pragma: keep, common header

#include "ecs/component.hpp"

struct Transform : Component
{
    glm::vec3 position;
};