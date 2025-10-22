#include "scene.hpp"

#include <vector>

// #include "components/transform.hpp"

Entity::Entity(EntityId id, ComponentMask mask)
{
	this->id = id;
	this->mask = mask;
}

EntityId Entity::get_id()
{
	return this->id;
}
ComponentMask Entity::get_mask()
{
	return this->mask;
}

void Entity::set_bit_in_mask(EntityInt position, bool value)
{
	this->mask.set(position, value);
}


ComponentPool::ComponentPool()
{
	pData = std::vector<void*>(ENTITY_START_CAPACITY);
}

void* ComponentPool::at(EntityInt index)
{
	return pData[index];
}

EntityInt ComponentPool::get_size()
{
	return pData.size();
}


Scene::Scene()
{
	allComponentPools = std::vector<ComponentPool>(MAX_COMPONENT_TYPES);
}

EntityId Scene::new_entity()
{
	EntityId entityId = static_cast<EntityId>(allEntities.size());
	Entity newEntity = Entity(entityId, ComponentMask());
    allEntities.push_back(newEntity);
    return entityId;
}