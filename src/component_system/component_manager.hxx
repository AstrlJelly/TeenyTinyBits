#pragma once
#include "component_manager.hpp"
#include "entity.hpp"
#include <cassert>


template<ComponentData T>
ComponentPool<T>::ComponentPool()
{
	componentsData = std::array<T, ENTITY_START_CAPACITY>();
}

template<ComponentData T>
T& ComponentPool<T>::set(EntityId_t index)
{
	componentsData[index] = {};
	return this->at(index);
}

template<ComponentData T>
T& ComponentPool<T>::at(EntityInt_t index)
{
	return componentsData.at(index);
}

template<ComponentData T>
EntityInt_t ComponentPool<T>::get_size()
{
	return componentsData.size();
}


template<ComponentData T>
EntityInt_t ComponentManager::get_component_id()
{
	static int32_t s_componentId = s_componentTypeCounter++;
	
	assert(s_componentId < MAX_COMPONENT_TYPES);

	return s_componentId;
}

template<ComponentData T>
T& ComponentManager::get_component(EntityId_t entityId)
{
	T componentId = get_component_id<T>();
	ComponentPool<T> componentPool = componentPools[componentId];
	return componentPool.at(entityId);
}

template<ComponentData T>
T& ComponentManager::add_component(EntityId_t entityId)
{
    int componentId = get_component_id<T>();

	// the array should be sized appropriately on construction
	// so, only necessary with dynamic sizing
	// if (allComponentPools.size() <= componentId)
	// {
	// 	allComponentPools.resize(allComponentPools.capacity() * 2);
	// }


	if (componentPools.at(componentId) == nullptr)
	{
		componentPools[componentId] = std::make_shared<ComponentPool<T>>();
	}

	ComponentPool<T>* componentPool = static_cast<ComponentPool<T>*>(componentPools.at(componentId).get());

	// Entity entity = allEntities.at(entityId);
	// entity.set_bit_in_mask(componentId, true);
	
	componentMasks.at(entityId).set(componentId, true);

	componentPool->set(entityId);
	return componentPool->at(entityId);
}