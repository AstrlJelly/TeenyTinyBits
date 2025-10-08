#pragma once

#include "glm/glm/glm.hpp" // IWYU pragma: keep, common header

class Transform
{
private:
    glm::vec3 position;

public:
    Transform();

    inline glm::vec3 get_position();
    inline void set_position(glm::vec3 newPos);
};