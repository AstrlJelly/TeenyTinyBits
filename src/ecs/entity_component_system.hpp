#pragma once

#include "ecs/component_manager.hpp"
#include "ecs/system_manager.hpp"
#include "ecs/entity_manager.hpp"

class ECSManager :
    public EntityManager,
    public ComponentManager,
    public SystemManager
{
private:
    bool running = true;

public:
    bool is_running();
    void start_exit();
};