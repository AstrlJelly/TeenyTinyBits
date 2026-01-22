#pragma once

#include "component_manager.hpp"
#include "system_manager.hpp"
#include "entity_manager.hpp"

#include "component.hpp"
#include "entity.hpp"

namespace teeny
{
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
            EntityId_t entityId = this->entityManager.new_entity();

            return entityId;
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
        DEFINE_PLURAL_COMPONENT_FUNC_RETURN(get_component, (EntityId_t entityId), (entityId))
        
        template<ComponentData T>
        T& add_component(EntityId_t entityId, T component = T())
        {
            auto& result = this->componentManager.add_component<T>(entityId, component);
            this->systemManager.entity_signature_changed(entityId, get_component_signature(entityId));
            return result;
        }
        DEFINE_PLURAL_COMPONENT_FUNC_RETURN(
            add_component, (EntityId_t entityId, TArgs... components), (entityId, components...))

        template<ComponentData T>
        [[nodiscard]] T& get_or_add_component(EntityId_t entityId, T component = T())
        {
            return this->componentManager.get_or_add_component<T>(entityId, component);
        }
        DEFINE_PLURAL_COMPONENT_FUNC_RETURN(
            get_or_add_component, (EntityId_t entityId, TArgs... components), (entityId, components...))

        template<ComponentData T>
        void remove_component(EntityId_t entityId)
        {
            this->componentManager.remove_component<T>(entityId);
            this->systemManager.entity_signature_changed(entityId, get_component_signature(entityId));
        }
        DEFINE_PLURAL_COMPONENT_FUNC_VOID(
            remove_component, (EntityId_t entityId), (entityId))

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
}
