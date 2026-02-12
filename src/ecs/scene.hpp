#pragma once

#include "ecs/component_signature.hpp"
#include "ecs/system.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glm/glm/ext/vector_float2.hpp"
#include "glm/glm/ext/vector_float3.hpp"

#include "component_manager.hpp"
#include "system_manager.hpp"
#include "entity_manager.hpp"

#include "component.hpp"
#include "entity.hpp"

namespace teeny
{
    class Scene
    {
      private:
        GLFWwindow* window;
        glm::vec3 clearColor = glm::vec3(0.165f, 0.165f, 0.2f);
        double deltaTime;
        double lastFrameTime;
        
        EntityManager    entityManager;
        ComponentManager componentManager;
        SystemManager    systemManager;
        bool running = true;

        inline constexpr static glm::vec2 DEFAULT_WINDOW_SIZE = glm::vec2(1280, 720);
        inline constexpr static const char* DEFAULT_WINDOW_TITLE = "Teeny";
        
    public:
        Scene();

        bool is_running();

        void update();
        void render();

        /**
        * @brief Sets the `running` variable to false, closing the program in a typical frame-loop
        *        
        */
        void lazy_exit();


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
        DEFINE_PLURAL_COMPONENT_FUNC_RETURN(get_components, (EntityId_t entityId), (entityId))
        
        template<ComponentData T>
        T& add_component(EntityId_t entityId, T component = T())
        {
            auto& result = this->componentManager.add_component<T>(entityId, component);
            this->systemManager.on_entity_signature_changed(entityId, this->get_component_signature(entityId));
            return result;
        }
        DEFINE_PLURAL_COMPONENT_FUNC_RETURN(
            add_components, (EntityId_t entityId, TArgs... components), (entityId, components...))

        template<ComponentData T>
        [[nodiscard]] T& get_or_add_component(EntityId_t entityId, T component = T())
        {
            return this->componentManager.get_or_add_component<T>(entityId, component);
        }
        DEFINE_PLURAL_COMPONENT_FUNC_RETURN(
            get_or_add_components, (EntityId_t entityId, TArgs... components), (entityId, components...))

        template<ComponentData T>
        void remove_component(EntityId_t entityId)
        {
            this->componentManager.remove_component<T>(entityId);
            this->systemManager.on_entity_signature_changed(entityId, this->get_component_signature(entityId));
        }
        DEFINE_PLURAL_COMPONENT_FUNC_VOID(
            remove_components, (EntityId_t entityId), (entityId))

        /**
        * @brief Template function to check for a component
        * 
        * @tparam TArgs Component type to check against
        * @param entityId The entity id to check for
        * @return bool True if entity contains component
        */
        template<ComponentData T>
        [[nodiscard]] bool has_component(EntityId_t entityId)
        {
            return this->componentManager.has_component<T>(entityId);
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
}
