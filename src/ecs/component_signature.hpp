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
        // constexpr ComponentSignature();

        explicit operator ComponentBitMask_t()
        {
            return mask;
        }
    
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
}
