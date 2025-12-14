#pragma once

#include <array>
#include <bitset>
#include <cstdint>
#include <type_traits>

#include <boost/preprocessor.hpp>


// define EntityInt so it's obvious when a number or id is being used
typedef int32_t EntityInt;
typedef EntityInt EntityId;

constexpr EntityInt ENTITY_START_CAPACITY = 0x0FFF;

constexpr EntityInt MAX_COMPONENT_TYPES = 32;
// can be replaced with `std::dynamic_bitset` if 'tis wished to be dynamic
typedef std::bitset<MAX_COMPONENT_TYPES> ComponentMask;

template<class T>
concept ComponentData = requires
{
    { T() };
    requires !std::is_arithmetic_v<T>;
    requires !std::is_pointer_v<T>;
};

struct Entity
{
private:
    EntityId id;
    ComponentMask mask;

public:
    Entity(EntityId id, ComponentMask mask);
    
    EntityId get_id();

    ComponentMask get_mask();
    void set_bit_in_mask(EntityInt position, bool value = true);
};

struct IComponentPool
{
    virtual ~IComponentPool() {};
};

template<ComponentData T>
class ComponentPool : public IComponentPool
{
private:
    // TODO: optimize with indexes so data can be cache hit (and for lower memory usage?)
    // std::vector<EntityInt> indexes;
    std::array<T, ENTITY_START_CAPACITY> componentsData;

public:
    ComponentPool<T>();

    // for the risky boys and girls
    // inline void* at(EntityId index);
    
    T& set(EntityId index);
    T& at (EntityId index);

    EntityInt get_size();
};

#include "component_system.hxx"
