#include "entity_component_system.hpp"
#include "ecs/system_manager.hpp"

namespace teeny
{
    bool ECSManager::is_running()
    {
        return this->running;
    }
    
    void ECSManager::lazy_exit()
    {
        this->running = false;
        teeny::println(Severity::IMPORTANT, "Exiting at end of current ECS iteration");
    }
}