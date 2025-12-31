#pragma once

#include <array>
#include <bitset>
#include <type_traits>


#include "components/component.hpp"
#include "entity.hpp"

using ComponentInt_t = uint32_t;
using ComponentId_t = ComponentInt_t;
constexpr ComponentId_t MAX_COMPONENT_TYPES = 32;

// using ComponentMask_t = std::bitset<MAX_COMPONENT_TYPES>;

template<class T>
concept ComponentData = requires
{
    // std::***_v is short for std::***::value
    requires std::is_base_of_v<Component, T>;
    requires !std::is_arithmetic_v<T>;
    requires !std::is_pointer_v<T>;
    { T() };
};

struct IComponentPool
{
    virtual ~IComponentPool() {};
};

template<ComponentData T>
class ComponentPool : public IComponentPool
{
private:
    // TODO: optimize with indices so data can be cache hit (and for lower memory usage?)
    std::array<EntityInt_t, ENTITY_START_CAPACITY> indices;
    std::array<T, ENTITY_START_CAPACITY> componentsData;

public:
    ComponentPool<T>();

    // for the risky boys and girls
    // inline void* at(EntityId index);
    
    T& set(EntityId_t idIndex);
    T& at (EntityId_t idIndex);

    EntityInt_t get_size();
};

class ComponentMask
{
private:
    // can be replaced with `std::dynamic_bitset` if 'tis wished to be dynamic
    using _ComponentBitMask_t = std::bitset<MAX_COMPONENT_TYPES>;
    _ComponentBitMask_t mask;
public:
    // declare private type to declare private variables with public type
    // while also not changing the formatting
    using ComponentBitMask_t = _ComponentBitMask_t;
    ComponentMask();

    template<ComponentData... TArgs> static ComponentMask from_components();

    /**
     * @brief Get the bitset object
     * 
     * @return `ComponentBitMask_t`
     */
    ComponentBitMask_t get_mask();

    /**
     * @brief Tests the bit at index `componentId`
     * @anchor at<T>
     * 
     * @tparam `T` : The `Component` to check for
     * @return `bool`, True if the index `Component` is on in mask
     */
    template<ComponentData T>
    bool at();
    /**
     * @ref at<T>
     * 
     * @param componentId 
     * @return true 
     * @return false 
     */
    bool at(ComponentId_t componentId);

    template<ComponentData T>
    void set(bool value);
    void set(ComponentId_t componentId, bool value);
};

class ComponentManager
{
private:
    std::array<ComponentMask, ENTITY_START_CAPACITY> componentMasks;
    std::array<IComponentPool*, MAX_COMPONENT_TYPES> componentPools;

    static inline ComponentInt_t s_componentTypeCounter = 0;

    template<ComponentData T>
    ComponentPool<T>& get_component_pool();
public:
    ComponentManager();
    ~ComponentManager();

    ComponentMask get_component_mask(EntityId_t entityId);

    /**
     * @brief Get the id associated with the `Component` `T`
     * 
     * @tparam `T` : `Component`
     * @return The `ComponentId_t` associated with `T`
     */
    template<ComponentData T> static ComponentId_t get_component_id();

    /**
     * @brief Get the component from inputted `entityId`
     * 
     * @tparam T Component 
     * @param entityId 
     * @return T&
     */
    template<ComponentData T> T& get_component(EntityId_t entityId);
    template<ComponentData T> T& add_component(EntityId_t entityId);

    /**
     * @brief Template function to check for 0 or more components
     * 
     * @tparam TArgs Component types to check against
     * @param entityId The entity id to check for
     * @return bool True if entity contains all components
     */
    template<ComponentData... TArgs> bool has_components(EntityId_t entityId);

};

#include "component_manager.hxx"
