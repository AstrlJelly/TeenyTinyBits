#include "system_manager.hpp"
#include "component_signature.hpp"
#include "entity.hpp"
#include "system.hpp"

namespace teeny
{
    void SystemManager::on_entity_signature_changed(EntityId_t entityId, ComponentSignature signature)
    {
        for (SystemId_t i = 0; i < this->systemsRegisteredState.size(); i++)
        {
            if (!systemsRegisteredState.test(i))
            {
                continue;
            }

            System& system = get_registered_system(i);
            system.entity_signature_changed(entityId, signature);
        }
    }
    
    bool SystemManager::is_system_registered(SystemId_t systemId)
    {
        return this->systemsRegisteredState.test(systemId);
    }
    
    [[nodiscard]] System& SystemManager::get_registered_system(SystemId_t systemId)
    {
        assert(systemId < registeredSystems.size());
        return this->registeredSystems.at(systemId);
    }
}