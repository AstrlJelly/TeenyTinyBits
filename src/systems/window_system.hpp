#pragma once

#include "ecs/entity_component_system.hpp"
#include "ecs/system.hpp"

#include "components/game_window.hpp"
#include "components/transform.hpp"


struct WindowSystem : System<Transform, GameWindow>
{
    void update(ECSManager& ecs);
    void render(ECSManager& ecs);
};