#pragma once

#include <array>
#include <bitset>

#include "ecs/entity.hpp"
#include "print.hpp"
#include "component_signature.hpp"
#include "system.hpp"

namespace teeny
{
    class SystemManager
    {
    private:
        std::bitset<MAX_SYSTEM_TYPES> systemsRegisteredState;
        std::array<System, MAX_SYSTEM_TYPES> registeredSystems;
    
    public:
        void on_entity_signature_changed(EntityId_t entityId, ComponentSignature signature);

        template<SystemType T>
        void register_entity(EntityId_t entityId)
        {
            T& registeredSystem = get_registered_system<T>();
            registeredSystem.register_entity(entityId);
        }
    
        template<SystemType T>
        T& register_system()
        {
            T& registeredSystem = get_registered_system<T>();
    
            if (!this->is_system_registered<T>())
            {
                SystemId_t systemId = get_system_id<T>();
                this->systemsRegisteredState.set(systemId);
            }
            else
            {
                teeny::println("The system \"", typeid(T).name(), "\" was already initialized.");
                teeny::println("Make sure to use SystemManager::get_registered_system() for existing systems!");
                teeny::println("NOTE: Still returning existing system.");
            }
            
            return registeredSystem;
        }
    
        template<SystemType T>
        bool is_system_registered()
        {
            return this->is_system_registered(get_system_id<T>());
        }
        bool is_system_registered(SystemId_t systemId);
    
        template<SystemType T> 
        [[nodiscard]] T& get_registered_system()
        {
            SystemId_t systemId = get_system_id<T>();
    
            return static_cast<T&>(this->get_registered_system(systemId));
        }
        [[nodiscard]] System& get_registered_system(SystemId_t systemId);
    };
}
