#include "entity_manager.hpp"

#include <format>
#include <stdexcept>

#include "entity.hpp"


EntityManager EntityManager::create()
{
	EntityManager base{};
	base.entityUsedStates = {};
	return base;
}

EntityId_t EntityManager::new_entity()
{
	// TODO: optimize this somehow, without increasing memory usage too much
	for (EntityInt_t i = 0; i < entityUsedStates.size(); i++)
	{
		if (!is_entity_used(i))
		{
    		return i;
		}
	}
	throw std::out_of_range(std::format("EntityManager ran out of entity ids. Was {} not enough?", entityUsedStates.size()));
}

inline bool EntityManager::is_entity_used(EntityId_t entityId)
{
	return entityUsedStates.test(entityId);
}