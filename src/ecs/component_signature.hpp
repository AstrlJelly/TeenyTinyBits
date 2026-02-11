#pragma once

#include <bitset>

#include "component.hpp"

namespace teeny
{
    class ComponentSignature
    {
        // can be replaced with `std::dynamic_bitset` if 'tis wished to be dynamic
    public: using ComponentBitMask_t = std::bitset<MAX_COMPONENT_TYPES>;
        
    private:
        ComponentBitMask_t mask;

    public:
        ComponentSignature();
        ComponentSignature(ComponentBitMask_t mask);
    
        template<ComponentData... TArgs>
        [[nodiscard]] constexpr static ComponentSignature from_components()
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
        [[nodiscard]] constexpr ComponentBitMask_t get_mask() const;
    
        /**
         * @brief Tests the bit at index `componentId`
         * @anchor at<T>
         * 
         * @tparam `T` : The `Component` to check for
         * @return `bool`, True if the index `Component` is on in mask
         */
        template<ComponentData T>
        [[nodiscard]] constexpr bool at()
        {
            return this->at(get_component_id<T>());
        }
        [[nodiscard]] constexpr bool at(ComponentId_t componentId);
    
        template<ComponentData T>
        constexpr void set(bool value = true)
        {
            this->set(get_component_id<T>(), value);
        }
        constexpr void set(ComponentId_t componentId, bool value = true);
    };
}
