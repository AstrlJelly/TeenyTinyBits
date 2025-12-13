#pragma once
#include "component_system.hpp"
#include "scene.hpp"
#include <memory>


template<class T>
EntityInt Scene::get_component_id()
{
	static int32_t s_componentId = s_componentTypeCounter++;
	return s_componentId;
}

template<class T>
T& Scene::get_component(EntityId entityId)
{
	T componentId = get_component_id<T>();
	ComponentPool<T> componentPool = allComponentPools[componentId];
	return std::weak_ptr<T>(&componentPool.at(entityId));
}

template<class T>
T& Scene::add_component(EntityId entityId)
{
    int componentId = get_component_id<T>();

	// the vector should be sized appropriately on construction
	// so, only necessary with dynamic sizing
	// if (allComponentPools.size() <= componentId)
	// {
	// 	allComponentPools.resize(allComponentPools.capacity() * 2);
	// }

	std::shared_ptr<IComponentPool> componentPool = (allComponentPools.at(componentId));

	// if new component (aka vector is uninitialized) make a new pool
	if (componentPool->get_size() <= 0)
	{
		allComponentPools[componentId] = std::unique_ptr<ComponentPool<T>>();
		componentPool = allComponentPools[componentId];
	}

	Entity entity = allEntities.at(entityId);
	entity.set_bit_in_mask(componentId, true);
	
	componentPool->set<T>(entityId);
	return componentPool->at<T>(entityId);
}