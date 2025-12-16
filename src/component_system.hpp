#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <type_traits>


#include "entity.hpp"

constexpr EntityInt_t MAX_COMPONENT_TYPES = 32;

// can be replaced with `std::dynamic_bitset` if 'tis wished to be dynamic
typedef std::bitset<MAX_COMPONENT_TYPES> ComponentMask_t;

template<class T>
concept ComponentData = requires
{
    { T() };
    requires !std::is_arithmetic_v<T>;
    requires !std::is_pointer_v<T>;
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
    
    T& set(EntityId_t index);
    T& at (EntityId_t index);

    EntityInt_t get_size();
};

class ComponentManager
{
private:
    std::array<ComponentMask_t, ENTITY_START_CAPACITY> componentMasks;
    std::array<std::shared_ptr<IComponentPool>, MAX_COMPONENT_TYPES> componentPools;

    static inline EntityInt_t s_componentTypeCounter = 0;
public:
    ComponentManager();

    // will get the id associated with the type inputted
    // stays consistent because of how static templates work
    template<ComponentData T> static EntityInt_t get_component_id();

    // i will do my best to not make this AWFULLY optimized like unity did
    template<ComponentData T> T& get_component(EntityId_t entityId);
    template<ComponentData T> T& add_component(EntityId_t id);

    ComponentMask_t get_component_mask(EntityId_t entityId);
};

#include "component_system.hxx"
