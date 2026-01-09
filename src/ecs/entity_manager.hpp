#pragma once

#include <bitset>

#include "entity.hpp"


// we either get the max of an unsigned integer or -1
// either cases will be handled elegantly
constexpr EntityId_t INVALID_ENTITY_ID = 0 - 1;

class EntityManager
{
private:
    std::bitset<ENTITY_START_CAPACITY> entityUsedStates;

public:
    EntityManager();

    EntityId_t new_entity();
    void destroy_entity(EntityId_t entityId);

    [[nodiscard]] inline bool is_entity_used(EntityId_t entityId);
};

