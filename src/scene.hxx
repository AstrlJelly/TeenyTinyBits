#pragma once
#include "scene.hpp"
#include <memory>

template<class T>
std::weak_ptr<T> ComponentPool::at(EntityInt index)
{
	return static_cast<T*>(pData[index]);
}


template<class T>
EntityInt Scene::get_component_id()
{
	static int32_t s_componentId = s_componentTypeCounter++;
	return s_componentId;
}

template<class T>
std::weak_ptr<T> Scene::get_component(EntityId entityId)
{
	T* component = allComponentPools[entityId];
	return component;
}

template<class T>
std::weak_ptr<T> Scene::assign(EntityId entityId)
{
    int componentId = get_component_id<T>();

	// the vector should be sized appropriately on construction
	// so, only necessary with dynamic sizing
	// if (allComponentPools.size() <= componentId)
	// {
	// 	allComponentPools.resize(allComponentPools.capacity() * 2);
	// }

	ComponentPool componentPool = allComponentPools[componentId];

	// if new component (aka vector is uninitialized) make a new pool
	if (componentPool.get_size() <= 0)
	{
		allComponentPools[componentId] = ComponentPool();
	}

	Entity entity = allEntities.at(entityId);

	// this was recommended; seems like it just breaks things?
	// T* pComponent = new (componentPool.at(entityId)) T();
    
    // heap allocation. i hope to remedy this
	std::shared_ptr<T> pComponent = std::make_shared<T>();

	entity.set_bit_in_mask(componentId, true);

	// the component should never be handled by a user
	return std::weak_ptr<T>(pComponent);
}