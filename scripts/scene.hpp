#pragma once

#include <bitset>
#include <cstdint>
#include <vector>

#include <boost/preprocessor.hpp>

// #define ALL_COMPONENT_TYPES Transform
// #define DEFINE_COMPONENT_FUNCTION(r, data, elem) 
// #define DEFINE_ALL_COMPONENT_FUNCTIONS() \
//     BOOST_PP_SEQ_FOR_EACH(DEFINE_COMPONENT_FUNCTION, _, BOOST_PP_VARIADIC_TO_SEQ(ALL_COMPONENT_TYPES))

// define EntityInt so it's obvious when a number or id is being used
typedef int32_t EntityInt;
typedef EntityInt EntityId;

constexpr EntityInt ENTITY_START_CAPACITY = 0x1000;

constexpr EntityInt MAX_COMPONENT_TYPES = 32;
// can be replaced with `std::vector<bool>` if 'tis wished to be dynamic
typedef std::bitset<MAX_COMPONENT_TYPES> ComponentMask;


class EntityDesc
{
private:
    EntityId id;
    ComponentMask mask;

public:
    EntityDesc(EntityId id, ComponentMask mask);
    
    EntityId get_id();
    ComponentMask get_mask();

    void set_bit_in_mask(EntityInt position, bool value = true);
};

class ComponentPool
{
private:
    // TODO: optimize with indexes so data can be cache hit
    // std::vector<EntityInt> indexes;
    std::vector<void*> pData;

public:
    ComponentPool();

    inline void* at(EntityInt index);
    template<class T>
    inline T* at(EntityInt index);

    inline EntityInt get_size();
};

class Scene
{
private:
    std::vector<EntityDesc> allEntities;
    std::vector<ComponentPool> allComponentPools;

    static inline EntityInt s_componentTypeCounter = 0;

public:
    Scene();

    // will get the id associated with the type inputted
    // stays consistent because of how templates work
    template<class T>
    EntityInt get_component_id();

    EntityId new_entity();

    template<class T>
    T* assign(EntityId id);
};
