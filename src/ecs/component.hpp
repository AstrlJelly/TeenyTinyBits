#pragma once

#include <cassert>
#include <cstdint>
#include <type_traits>


using ComponentInt_t = uint32_t;
using ComponentId_t = ComponentInt_t;
constexpr ComponentInt_t MAX_COMPONENT_TYPES = 0x80;

class Component{};

template<class T>
concept ComponentData = requires
{
    // std::***_v is short for std::***::value
    requires std::is_base_of_v<Component, T>;
    requires !std::is_arithmetic_v<T>;
    requires !std::is_pointer_v<T>;
    { T() };
};

/**
  * @brief Get the id associated with the `Component` `T`
  * 
  * @tparam `T` : `Component`
  * @return The `ComponentId_t` associated with `T`
  */
template<ComponentData T>
[[nodiscard]] static constexpr ComponentId_t get_component_id()
{
	static ComponentInt_t s_componentTypeCounter = 0;
	static ComponentInt_t s_componentId = s_componentTypeCounter++;
	
	assert(s_componentId < MAX_COMPONENT_TYPES);

	return s_componentId;
}