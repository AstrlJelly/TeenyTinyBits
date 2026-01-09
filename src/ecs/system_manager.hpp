#pragma once

#include <array>
#include <bitset>
#include <print>

#include "ecs/system.hpp"

class SystemManager
{
private:
    std::bitset<MAX_SYSTEM_TYPES> systemsRegisteredState;
    std::array<System<>, MAX_SYSTEM_TYPES> registeredSystems;

public:
    template<SystemType T>
    T& register_system()
    {
        T& registeredSystem = get_registered_system<T>();

        if (this->is_system_registered<T>())
        {
            std::println("The system \"{}\" was already initialized.", typeid(T).name());
            std::println("Make sure to use SystemManager::get_registered_system() for existing systems!");
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
        assert(systemId < registeredSystems.size());

        // HACK (possibly): two systems are basically the same, data-wise
        return reinterpret_cast<T&>(registeredSystems.at(systemId));
    }
};
