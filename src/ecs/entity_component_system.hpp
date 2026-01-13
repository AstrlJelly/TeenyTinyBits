#pragma once

#include "component_manager.hpp"
#include "system_manager.hpp"
#include "entity_manager.hpp"

#include "component.hpp"
#include "entity.hpp"

class ECSManager
{
private:
    EntityManager    entityManager;
    ComponentManager componentManager;
    SystemManager    systemManager;
    bool running = true;

public:
    bool is_running();
    void start_exit();


    // entity manager
    EntityId_t new_entity()
    {
        return this->entityManager.new_entity();
    }
    void destroy_entity(EntityId_t entityId)
    {
        return this->entityManager.destroy_entity(entityId);
    }

    [[nodiscard]] bool is_entity_used(EntityId_t entityId)
    {
        return this->entityManager.is_entity_used(entityId);
    }



    // component manager
    [[nodiscard]] ComponentSignature get_component_signature(EntityId_t entityId)
    {
        return this->componentManager.get_component_signature(entityId);
    }

    /**
     * @brief Get the component from inputted `entityId`
     * 
     * @tparam T Component
     * @param entityId 
     * @return T&
     */
    template<ComponentData T>
    [[nodiscard]] T& get_component(EntityId_t entityId)
    {
        return this->componentManager.get_component<T>(entityId);
    }
    template<ComponentData T>
    T& add_component(EntityId_t entityId)
    {
        auto& result = this->componentManager.add_component<T>(entityId);
        this->systemManager.entity_signature_changed(entityId, get_component_signature(entityId));
        return result;
    }
    template<ComponentData T>
    [[nodiscard]] T& get_or_add_component(EntityId_t entityId)
    {
        return this->componentManager.get_or_add_component<T>(entityId);
    }
    template<ComponentData T>
    T& remove_component(EntityId_t entityId)
    {
        auto result = this->componentManager.remove_component<T>(entityId);
        this->systemManager.entity_signature_changed(entityId, get_component_signature(entityId));
        return result;
    }

    /**
     * @brief Template function to check for 0 or more components
     * 
     * @tparam TArgs Component types to check against
     * @param entityId The entity id to check for
     * @return bool True if entity contains all components
     */
    template<ComponentData... TArgs>
    [[nodiscard]] bool has_components(EntityId_t entityId)
    {
        return this->componentManager.has_components<TArgs...>(entityId);
    }



    // system manager
    template<SystemType T>
    T& register_system()
    {
        return this->systemManager.register_system<T>();
    }

    template<SystemType T>
    bool is_system_registered()
    {
        return this->systemManager.is_system_registered<T>();
    }
    bool is_system_registered(SystemId_t systemId)
    {
        return this->systemManager.is_system_registered(systemId);
    }

    template<SystemType T> 
    [[nodiscard]] T& get_registered_system()
    {
        return this->systemManager.get_registered_system<T>();
    }
};