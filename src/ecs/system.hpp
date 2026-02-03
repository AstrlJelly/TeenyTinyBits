#pragma once

#include <cassert>
#include <set>
#include <type_traits>

#include "component_signature.hpp"
#include "entity.hpp"


#define SET_COMPONENT_SIGNATURE(args...) ComponentSignature matchingSignature = \
    ComponentSignature::from_components<args>()

namespace teeny
{
    using SystemInt_t = uint32_t;
    using SystemId_t = SystemInt_t;
    constexpr SystemInt_t MAX_SYSTEM_TYPES = 0x80;
    
    class System
    {
    protected:
        ComponentSignature matchingSignature;

        // todo: check if an array is more efficient (memory usage vs cache hits basically)
        std::set<EntityId_t> matchingEntities;
    
    public:
        inline EntityInt_t get_entity_count()
        {
            return matchingEntities.size();
        }

        void entity_signature_changed(EntityId_t entityId, ComponentSignature signature)
        {
            ComponentSignature::ComponentBitMask_t systemMask = this->matchingSignature.get_mask();
            bool isSignatureMatching = (systemMask & signature.get_mask()) == systemMask;
            bool entityInSystem = this->matchingEntities.contains(entityId);

            if (isSignatureMatching && !entityInSystem)
            {
                this->matchingEntities.insert(entityId);
            }
            else if (!isSignatureMatching && entityInSystem)
            {
                this->matchingEntities.erase(entityId);
            }
        }
    };
    
    
    template<class T>
    concept SystemType = requires {
        std::is_same_v<System, T> ||
            std::is_base_of_v<System, T>;
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
}