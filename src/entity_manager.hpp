#pragma once

#include <bitset>

#include "entity.hpp"


class EntityManager
{
private:
    std::bitset<ENTITY_START_CAPACITY> entityUsedStates;

public:
    EntityManager create();

    EntityId_t new_entity();

    inline bool is_entity_used(EntityId_t entityId);
};

