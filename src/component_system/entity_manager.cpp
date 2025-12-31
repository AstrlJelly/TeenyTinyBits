#include "entity_manager.hpp"

#include <format>
#include <stdexcept>

#include "entity.hpp"


EntityManager::EntityManager()
{
	this->entityUsedStates = {};
}

EntityId_t EntityManager::new_entity()
{
	// TODO: optimize this somehow, without increasing memory usage too much
	for (EntityInt_t i = 0; i < this->entityUsedStates.size(); i++)
	{
		if (!is_entity_used(i))
		{
    		return i;
		}
	}
	throw std::out_of_range(std::format("EntityManager ran out of entity ids. Was {} not enough?", this->entityUsedStates.size()));
}

inline bool EntityManager::is_entity_used(EntityId_t entityId)
{
	return this->entityUsedStates.test(entityId);
}