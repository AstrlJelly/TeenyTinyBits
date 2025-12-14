#pragma once

#include "component_system.hpp"
#include "glm/glm/glm.hpp" // IWYU pragma: keep, common header


struct Transform 
{
    glm::vec3 position;
};
static_assert(ComponentData<Transform>);