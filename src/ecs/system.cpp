#include "system.hpp"
#include "ecs/component_signature.hpp"
#include "ecs/entity.hpp"

#include <cassert>
#include <set>


namespace teeny
{
    EntityInt_t System::get_entity_count()
    {
        return this->matchingEntities.size();
    }

    void System::register_entity(EntityId_t entityId)
    {
        this->matchingEntities.insert(entityId);
    }

    bool System::is_entity_in_system(EntityId_t entityId)
    {
        return this->matchingEntities.contains(entityId);
    }

    ComponentSignature System::entity_signature_changed(EntityId_t entityId, ComponentSignature signature)
    {
        if (this->is_entity_in_system(entityId))
        {
            signature = signature.get_mask() | this->requiredComponents.get_mask();
        }
        return signature;
    }
}