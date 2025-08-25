#include "components/transform.hpp"

Transform::Transform()
{
    position = {};
}

glm::vec3 Transform::get_position()
{
    return position;
}
void Transform::set_position(glm::vec3 newPos)
{
    position = newPos;
}