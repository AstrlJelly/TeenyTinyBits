#pragma once

#include "ecs/component_signature.hpp"
#include "ecs/entity.hpp"
#include <cassert>
#include <cstdint>
#include <set>
#include <type_traits>



#define SET_COMPONENT_SIGNATURE(...) ComponentSignature matchingSignature = \
    ComponentSignature::from_components<__VA_ARGS__>()

namespace teeny
{
    using SystemInt_t = uint32_t;
    using SystemId_t = SystemInt_t;
    constexpr SystemInt_t MAX_SYSTEM_TYPES = 0x80;
    
    class System
    {
    protected:
        ComponentSignature requiredComponents;

        // todo: check if an array is more efficient (memory usage vs cache hits basically)
        std::set<EntityId_t> matchingEntities;
    
    public:
        EntityInt_t get_entity_count();

        void register_entity(EntityId_t entityId);

        bool is_entity_in_system(EntityId_t entityId);

        ComponentSignature entity_signature_changed(EntityId_t entityId, ComponentSignature signature);
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