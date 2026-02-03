#pragma once

#include "ecs/entity_component_system.hpp"
#include "ecs/system.hpp"

#include "ecs/components/game_window.hpp"


namespace teeny
{
    struct WindowSystem : public System
    {
        SET_COMPONENT_SIGNATURE(GameWindow);
    
        void update(ECSManager& ecs);
        void render(ECSManager& ecs);
    };
}