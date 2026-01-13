#pragma once

#include <bitset>

#include "component.hpp"

class ComponentSignature
{
private:
    // can be replaced with `std::dynamic_bitset` if 'tis wished to be dynamic
    using _ComponentBitMask_t = std::bitset<MAX_COMPONENT_TYPES>;
    _ComponentBitMask_t mask;
public:
    // declare private type to declare private variables with public type
    // while also not changing the formatting
    using ComponentBitMask_t = _ComponentBitMask_t;
    // constexpr ComponentSignature();

    template<ComponentData... TArgs>
    [[nodiscard]] inline constexpr static ComponentSignature from_components()
    {
        ComponentSignature base;
        (base.set<TArgs>(true), ...);
        return base;
    }

    /**
     * @brief Get the bitset object
     * 
     * @return `ComponentBitMask_t`
     */
    [[nodiscard]] constexpr ComponentBitMask_t get_mask() const
    {
        return this->mask;
    }

    /**
     * @brief Tests the bit at index `componentId`
     * @anchor at<T>
     * 
     * @tparam `T` : The `Component` to check for
     * @return `bool`, True if the index `Component` is on in mask
     */
    template<ComponentData T>
    [[nodiscard]] constexpr inline bool at()
    {
	    return this->at(get_component_id<T>());
    }
    [[nodiscard]] constexpr inline bool at(ComponentId_t componentId)
    {
	    return this->mask.test(componentId);
    }

    template<ComponentData T>
    constexpr inline void set(bool value = true)
    {
        this->set(get_component_id<T>(), value);
    }
    constexpr inline void set(ComponentId_t componentId, bool value = true)
    {
        this->mask.set(componentId, value);
    }
};
