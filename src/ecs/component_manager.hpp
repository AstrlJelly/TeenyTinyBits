#pragma once

#include <array>
#include <tuple>

#include "component_pool.hpp"
#include "component_signature.hpp"
#include "entity.hpp"
#include "component.hpp"


#define DEFINE_PLURAL_COMPONENT_FUNC(returnType, funcName, defArgs, callArgs)\
    template<ComponentData... TArgs>\
    returnType funcName defArgs\
    { return this->funcName<TArgs...> callArgs; }

#define DEFINE_PLURAL_COMPONENT_FUNC_RETURN(funcName, defArgs, callArgs)\
    DEFINE_PLURAL_COMPONENT_FUNC(std::tuple<TArgs&...>, funcName, defArgs, callArgs)
#define DEFINE_PLURAL_COMPONENT_FUNC_VOID(funcName, defArgs, callArgs)\
    DEFINE_PLURAL_COMPONENT_FUNC(void, funcName, defArgs, callArgs)

namespace teeny
{
    class ComponentManager
    {
    private:
        std::array<ComponentSignature, ENTITY_START_CAPACITY> componentSignatures;
        // unfortunately, the component pool must be a pointer
        // components are of various sizes therefore pools are too
        std::array<IComponentPool*, MAX_COMPONENT_TYPES> componentPools;
    
        template<ComponentData T>
        [[nodiscard]] ComponentPool<T>& get_component_pool()
        {
            IComponentPool& componentPool = this->get_component_pool(get_component_id<T>());
            return static_cast<ComponentPool<T>&>(componentPool);
        }
        [[nodiscard]] IComponentPool& get_component_pool(ComponentId_t componentId);
        
    public:
        ComponentManager();
        ~ComponentManager();
    
        [[nodiscard]] ComponentSignature& get_component_signature(EntityId_t entityId);
    
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
            return this->get_component_pool<T>().at(entityId);
        }
        DEFINE_PLURAL_COMPONENT_FUNC_RETURN(get_component, (EntityId_t entityId), (entityId))
    
        template<ComponentData T>
        T& add_component(EntityId_t entityId, T component = T())
        {
            ComponentId_t componentId = get_component_id<T>();
    
            if (componentPools.at(componentId) == nullptr)
            {
                componentPools[componentId] = new ComponentPool<T>();
            }
    
            ComponentPool<T>& componentPool = this->get_component_pool<T>();
    
            ComponentSignature& componentSignature = componentSignatures.at(entityId);
            componentSignature.set(componentId, true);
    
            componentPool.set(entityId, component);
            return componentPool.at(entityId);
        }
        DEFINE_PLURAL_COMPONENT_FUNC_RETURN(
            add_component, (EntityId_t entityId, TArgs... components), (entityId, components...))
    
        template<ComponentData T>
        [[nodiscard]] T& get_or_add_component(EntityId_t entityId, T component = T())
        {
            if (!has_components<T>(entityId))
            {
                return add_component<T>(entityId, component);
            }
            
            return this->get_component_pool<T>().at(entityId);
        }
        DEFINE_PLURAL_COMPONENT_FUNC_RETURN(
            get_or_add_component, (EntityId_t entityId, TArgs... components), (entityId, components...))
    
        // TODO: this
        template<ComponentData T>
        void remove_component(EntityId_t entityId)
        {
            ComponentId_t componentId = get_component_id<T>();
    
            if (componentPools.at(componentId) == nullptr)
            {
                
            }
    
            ComponentPool<T>& componentPool = this->get_component_pool<T>();
    
            ComponentSignature& componentSignature = this->get_component_signature(entityId);
            componentSignature.set(entityId, false);
    
            componentPool.set(entityId);
            return componentPool.at(entityId);
        }
        DEFINE_PLURAL_COMPONENT_FUNC_VOID(
            remove_component, (EntityId_t entityId), (entityId))
    
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
            return this->has_components<T>(entityId);
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
            ComponentSignature componentSignature = ComponentSignature::from_components<TArgs...>();
            return this->has_components(entityId, componentSignature);
        }
        /**
         * @brief Function to check for 0 or more components
         * 
         * @tparam TArgs Component types to check against
         * @param entityId The entity id to check for
         * @return bool True if entity contains all components
         */
        [[nodiscard]] bool has_components(EntityId_t entityId, ComponentSignature componentSignature);
    };
}
