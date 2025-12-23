#pragma once

#include "component_manager.hpp"
#include "entity_manager.hpp"

class Scene
{
private:
    EntityManager entityManager;
    ComponentManager componentManager;
public:
    Scene();
    static Scene create();
};