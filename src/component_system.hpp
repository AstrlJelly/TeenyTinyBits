#pragma once

#include <bitset>
#include <cstdint>
#include <vector>

#include <boost/preprocessor.hpp>


// define EntityInt so it's obvious when a number or id is being used
typedef int32_t EntityInt;
typedef EntityInt EntityId;

constexpr EntityInt ENTITY_START_CAPACITY = 0x0FFF;

constexpr EntityInt MAX_COMPONENT_TYPES = 32;
// can be replaced with `std::dynamic_bitset` if 'tis wished to be dynamic
typedef std::bitset<MAX_COMPONENT_TYPES> ComponentMask;


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

class IComponentPool
{
public:
    template<class T>
    inline void set(EntityId index);
    template<class T>
    inline T at(EntityId index);

    virtual EntityInt get_size();
};

template<class T>
class ComponentPool : public IComponentPool
{
private:
    // TODO: optimize with indexes so data can be cache hit (and for lower memory usage?)
    // std::vector<EntityInt> indexes;
    std::vector<T> componentsData;

public:
    ComponentPool();

    // for the risky boys and girls
    // inline void* at(EntityId index);
    
    inline T set(EntityId index);
    inline T at(EntityId index);

    EntityInt get_size();
};

#include "component_system.hxx"
