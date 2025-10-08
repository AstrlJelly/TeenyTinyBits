#include "scene.hpp"

#include <vector>

#include "components/transform.hpp"

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

template<class T>
T* ComponentPool::at(EntityInt index)
{
	return static_cast<T*>(pData[index]);
}

EntityInt ComponentPool::get_size()
{
	return pData.size();
}


Scene::Scene()
{
	allComponentPools = std::vector<ComponentPool>(MAX_COMPONENT_TYPES);
}

template<class T>
int32_t Scene::get_component_id()
{
	static int32_t s_componentId = s_componentTypeCounter++;
	return s_componentId;
}

EntityId Scene::new_entity()
{
	EntityId entityId = static_cast<EntityId>(allEntities.size());
	EntityDesc newEntity = EntityDesc(entityId, ComponentMask());
    allEntities.push_back(newEntity);
    return entityId;
}

template<class T>
T* Scene::assign(EntityId entityId)
{
    int componentId = get_component_id<T>();

	// the vector should be sized appropriately on construction
	// so, only necessary with dynamic sizing
	// if (allComponentPools.size() <= componentId)
	// {
	// 	allComponentPools.resize(allComponentPools.capacity() * 2);
	// }

	ComponentPool componentPool = allComponentPools[componentId];

	// if new component, make a new pool
	if (componentPool.get_size() <= 0)
	{
		allComponentPools[componentId] = ComponentPool();
	}

	EntityDesc entity = allEntities.at(entityId);

	// this was recommended; seems like it just breaks things?
	// T* pComponent = new (componentPool.at(entityId)) T();
	T* pComponent = new T();

	entity.set_bit_in_mask(componentId, true);
	return pComponent;
}