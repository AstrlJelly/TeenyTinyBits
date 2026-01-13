#pragma once

#include <array>

#include "component_pool.hpp"
#include "component_signature.hpp"
#include "entity.hpp"
#include "component.hpp"


class ComponentManager
{
private:
    std::array<ComponentSignature, ENTITY_START_CAPACITY> componentSignatures;
    std::array<IComponentPool*, MAX_COMPONENT_TYPES> componentPools;

    static inline ComponentInt_t s_componentTypeCounter = 0;

    template<ComponentData T>
    [[nodiscard]] ComponentPool<T>& get_component_pool()
    {
        IComponentPool& componentPool = ComponentManager::get_component_pool(get_component_id<T>());
        return static_cast<ComponentPool<T>&>(componentPool);
    }
    [[nodiscard]] IComponentPool& get_component_pool(ComponentId_t componentId);
public:
    ComponentManager();
    ~ComponentManager();

    [[nodiscard]] ComponentSignature get_component_signature(EntityId_t entityId);

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
    template<ComponentData T>
    T& add_component(EntityId_t entityId)
    {
        ComponentId_t componentId = get_component_id<T>();

        if (componentPools.at(componentId) == nullptr)
        {
            componentPools[componentId] = new ComponentPool<T>();
        }

        ComponentPool<T>& componentPool = this->get_component_pool<T>();

        ComponentSignature& componentSignature = componentSignatures.at(entityId);
        componentSignature.set(entityId, true);

        componentPool.set(entityId);
        return componentPool.at(entityId);
    }
    template<ComponentData T>
    [[nodiscard]] T& get_or_add_component(EntityId_t entityId)
    {
        if (!has_components<T>(entityId))
        {
            return add_component<T>(entityId);
        }
        
        return this->get_component_pool<T>().at(entityId);
    }
    template<ComponentData T>
    void remove_component(EntityId_t entityId)
    {
        ComponentId_t componentId = get_component_id<T>();

        if (componentPools.at(componentId) == nullptr)
        {
            
        }

        ComponentPool<T>& componentPool = this->get_component_pool<T>();

        ComponentSignature& componentSignature = componentSignatures.at(entityId);
        componentSignature.set(entityId, true);

        componentPool.set(entityId);
        return componentPool.at(entityId);
    }

    /**
     * @brief Template function to check for 0 or more components
     * 
     * @tparam TArgs Component types to check against
     * @param entityId The entity id to check for
     * @return bool True if entity contains all components
     */
    template<ComponentData... TArgs>
    [[nodiscard]] constexpr bool has_components(EntityId_t entityId)
    {
        ComponentSignature entityComponents = this->get_component_signature(entityId);
        ComponentSignature componentSignature = ComponentSignature::from_components<TArgs...>();
        return (entityComponents.get_mask() & componentSignature.get_mask()) == componentSignature.get_mask();
    }
};
