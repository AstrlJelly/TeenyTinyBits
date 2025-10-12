#include "scene.hpp"

#include <vector>

// #include "components/transform.hpp"

EntityDesc::EntityDesc(EntityId id, ComponentMask mask)
{
	this->id = id;
	this->mask = mask;
}

EntityId EntityDesc::get_id()
{
	return this->id;
}
ComponentMask EntityDesc::get_mask()
{
	return this->mask;
}

void EntityDesc::set_bit_in_mask(EntityInt position, bool value)
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
	EntityDesc newEntity = EntityDesc(entityId, ComponentMask());
    allEntities.push_back(newEntity);
    return entityId;
}