#include "entity_manager.hpp"

#include <format>
#include <stdexcept>

#include "entity.hpp"


EntityManager::EntityManager()
{
}

// TODO: optimize this somehow, without increasing memory usage too much
EntityId_t EntityManager::new_entity()
{
	for (EntityId_t i = 0; i < this->entityUsedStates.size(); i++)
	{
		if (!this->is_entity_used(i))
		{
			this->entityUsedStates.set(i, true);
    		return i;
		}
	}
	throw std::out_of_range(std::format("EntityManager ran out of entity ids. Was {} not enough?", this->entityUsedStates.size()));
}

void EntityManager::destroy_entity(EntityId_t entityId)
{
	this->entityUsedStates.set(entityId, false);
}

bool EntityManager::is_entity_used(EntityId_t entityId)
{
	return this->entityUsedStates.test(entityId);
}