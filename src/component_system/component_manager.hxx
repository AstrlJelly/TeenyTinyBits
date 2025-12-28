#pragma once
#include "component_manager.hpp"
#include "entity.hpp"
#include <cassert>


template<ComponentData T>
ComponentPool<T>::ComponentPool()
{
	indices = std::array<EntityInt_t, ENTITY_START_CAPACITY>();
	componentsData = std::array<T, ENTITY_START_CAPACITY>();
}

template<ComponentData T>
T& ComponentPool<T>::set(EntityId_t idIndex)
{
	componentsData[idIndex] = {};
	return this->at(idIndex);
}

template<ComponentData T>
T& ComponentPool<T>::at(EntityId_t idIndex)
{
	return componentsData.at(idIndex);
}

template<ComponentData T>
EntityInt_t ComponentPool<T>::get_size()
{
	return componentsData.size();
}


template<ComponentData T>
ComponentId_t ComponentManager::get_component_id()
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
    ComponentId_t componentId = get_component_id<T>();

	// the array should be sized appropriately on construction
	// so, only necessary with dynamic sizing
	// if (allComponentPools.size() <= componentId)
	// {
	// 	allComponentPools.resize(allComponentPools.capacity() * 2);
	// }


	if (componentPools.at(componentId) == nullptr)
	{
		componentPools[componentId] = new ComponentPool<T>();
	}

	// HACK: find a better way to access the component pool list? (if there is a better way)
	ComponentPool<T>* componentPool = static_cast<ComponentPool<T>*>(componentPools.at(componentId));

	componentMasks.at(entityId).set(componentId, true);

	componentPool->set(entityId);
	return componentPool->at(entityId);
}