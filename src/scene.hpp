#pragma once

#include <bitset>
#include <cstdint>
#include <vector>

#include <boost/preprocessor.hpp>

// yikes.

// #define ALL_COMPONENT_TYPES Transform, Test

// #define DEFINE_COMPONENT_FUNCTION_1_REPLACE(r, data, elem) \
//     BOOST_PP_SEQ_FOR_EACH(BUILD_COMPONENT_FUNCTION, elem, data)

// // every comma will be replaced with the component type
// // is this janky? idk. does it make templates work? yessssss :333
// // edit: nooooooo(t yet). :[[[
// #define DEFINE_ALL_COMPONENT_FUNCTIONS(...) \
//     BOOST_PP_SEQ_FOR_EACH(DEFINE_COMPONENT_FUNCTION, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__), BOOST_PP_VARIADIC_TO_SEQ(ALL_COMPONENT_TYPES))

// DEFINE_ALL_COMPONENT_FUNCTIONS(template);


// define EntityInt so it's obvious when a number or id is being used
typedef int32_t EntityInt;
typedef EntityInt EntityId;

constexpr EntityInt ENTITY_START_CAPACITY = 0xFFFF;

constexpr EntityInt MAX_COMPONENT_TYPES = 32;
// can be replaced with `std::dynamic_bitset` if 'tis wished to be dynamic
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

// simple wrapper around a void pointer vector for typez
class ComponentPool
{
private:
    // TODO: optimize with indexes so data can be cache hit (and for lower memory usage?)
    // std::vector<EntityInt> indexes;
    std::vector<void*> pData;

public:
    ComponentPool();

    inline void* at(EntityInt index);
    template<class T>
    inline T* at(EntityInt index);

    EntityInt get_size();
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

    // i will do my best to not make this AWFULLY optimized like unity did
    template<class T>
    T* get_component();

    template<class T>
    T* assign(EntityId id);
};

#include "scene.hxx"
