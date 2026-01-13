#pragma once

#include <cassert>
#include <set>
#include <type_traits>

#include "component.hpp"
#include "component_signature.hpp"
#include "entity.hpp"


using SystemInt_t = uint32_t;
using SystemId_t = SystemInt_t;
constexpr SystemInt_t MAX_SYSTEM_TYPES = 0x80;

struct System
{
    ComponentSignature matchingSignature;
    // TODO: check if an array is more efficient (memory vs cache basically)
    std::set<EntityId_t> matchingEntities;

    
};

#define SET_COMPONENT_SIGNATURE(args...) ComponentSignature matchingSignature = \
    ComponentSignature::from_components<args>()


template<class T>
concept SystemType = requires {
    std::is_same_v<System, T> ||
        std::is_base_of_v<System, T>;
    sizeof(System) == sizeof(T);
};


/**
  * @brief Get the id associated with the `System` `T`
  * 
  * @tparam `T` : `System`
  * @return The `SystemId_t` associated with `T`
  */
template<SystemType T>
[[nodiscard]] static constexpr SystemId_t get_system_id()
{
    static SystemInt_t s_systemTypeCounter = 0;
    static SystemId_t s_systemId = s_systemTypeCounter++;

    assert(s_systemId < MAX_SYSTEM_TYPES);

    return s_systemId;
}