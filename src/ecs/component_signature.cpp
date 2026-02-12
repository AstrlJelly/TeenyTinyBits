#include "component_signature.hpp"

#include "component.hpp"

namespace teeny
{
    // /**
    //     * @brief Get the bitset object
    //     * 
    //     * @return `ComponentBitMask_t`
    //     */
    [[nodiscard]] constexpr bool ComponentSignature::at(ComponentId_t componentId)
    {
        return this->mask.test(componentId);
    }
    
    constexpr void ComponentSignature::set(ComponentId_t componentId, bool value)
    {
        this->mask.set(componentId, value);
    }
}
