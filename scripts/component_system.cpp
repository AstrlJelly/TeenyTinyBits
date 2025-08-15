#include "component_system.hpp"

ComponentSystem* ComponentSystem::get_instance()
{
    return ComponentSystem::instance;
}