#include "system_manager.hpp"

bool SystemManager::is_system_registered(SystemId_t systemId)
{
    return this->systemsRegisteredState.test(systemId);
}