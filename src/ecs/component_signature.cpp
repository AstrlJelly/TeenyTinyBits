#include "component_signature.hpp"

#include "component.hpp"

namespace teeny
{
    ComponentSignature::ComponentSignature() {}
    ComponentSignature::ComponentSignature(ComponentSignature::ComponentBitMask_t mask)
    {
        this->mask = mask;
    }

    /**
        * @brief Get the bitset object
        * 
        * @return `ComponentBitMask_t`
        */
    [[nodiscard]] constexpr ComponentSignature::ComponentBitMask_t ComponentSignature::get_mask() const
    {
        return this->mask;
    }
    [[nodiscard]] constexpr bool ComponentSignature::at(ComponentId_t componentId)
    {
        return this->mask.test(componentId);
    }
    
    constexpr void ComponentSignature::set(ComponentId_t componentId, bool value)
    {
        this->mask.set(componentId, value);
    }
}
