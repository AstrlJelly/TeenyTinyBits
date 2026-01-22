#include "system_manager.hpp"
#include "component_signature.hpp"
#include "entity.hpp"
#include "system.hpp"

namespace teeny
{
    void SystemManager::entity_signature_changed(EntityId_t entityId, ComponentSignature signature)
    {
        for (SystemId_t i = 0; i < this->systemsRegisteredState.size(); i++)
        {
            if (systemsRegisteredState.test(i))
            {
                System& system = get_registered_system(i);
                
                auto systemMask = system.matchingSignature.get_mask();
                bool isSignatureMatching = (systemMask & signature.get_mask()) == systemMask;
                bool entityInSystem = system.matchingEntities.contains(entityId);
                if (isSignatureMatching != entityInSystem)
                {
                    if (entityInSystem)
                    {
                        system.matchingEntities.erase(entityId);
                    }
                    else
                    {
                        system.matchingEntities.insert(entityId);
                    }
                }
            }
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