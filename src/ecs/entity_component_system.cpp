#include "entity_component_system.hpp"

bool ECSManager::is_running()
{
    return this->running;
}

void ECSManager::start_exit()
{
    this->running = false;
}