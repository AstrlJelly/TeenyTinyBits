#pragma once

#include <memory>

#include "component_system.hpp"
#include "entity_manager.hpp"


class Scene
{
private:
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<ComponentManager> componentManager;

public:
    

};