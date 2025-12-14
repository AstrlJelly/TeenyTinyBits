#pragma once
#include "component_system.hpp"
#include "scene.hpp"
#include <memory>


template<ComponentData T>
EntityInt Scene::get_component_id()
{
	static int32_t s_componentId = s_componentTypeCounter++;
	return s_componentId;
}

template<ComponentData T>
T& Scene::get_component(EntityId entityId)
{
	T componentId = get_component_id<T>();
	ComponentPool<T> componentPool = allComponentPools[componentId];
	return &componentPool.at(entityId);
}

template<ComponentData T>
T& Scene::add_component(EntityId entityId)
{
    int componentId = get_component_id<T>();

	// the array should be sized appropriately on construction
	// so, only necessary with dynamic sizing
	// if (allComponentPools.size() <= componentId)
	// {
	// 	allComponentPools.resize(allComponentPools.capacity() * 2);
	// }


	if (allComponentPools.at(componentId) == nullptr)
	{
		allComponentPools[componentId] = std::make_shared<ComponentPool<T>>();
	}

	ComponentPool<T>* componentPool = static_cast<ComponentPool<T>*>(allComponentPools.at(componentId).get());

	Entity entity = allEntities.at(entityId);
	entity.set_bit_in_mask(componentId, true);
	
	componentPool->set(entityId);
	return componentPool->at(entityId);
}