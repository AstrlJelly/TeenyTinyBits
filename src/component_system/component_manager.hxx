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


template<ComponentData... TArgs>
ComponentMask ComponentMask::from_components()
{
	ComponentMask base{};
	(base.set<TArgs>(true), ...);
	return base;
}


template<ComponentData T>
bool ComponentMask::at()
{
	return this->at(ComponentManager::get_component_id<T>());
}
template<ComponentData T>
void ComponentMask::set(bool value)
{
	this->set(ComponentManager::get_component_id<T>(), value);
}


template<ComponentData T>
ComponentPool<T>& ComponentManager::get_component_pool()
{
	return static_cast<ComponentPool<T>&>(componentPools.at(ComponentManager::get_component_id<T>()));
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
	return this->get_component_pool<T>().at(entityId);
}

template<ComponentData T>
T& ComponentManager::add_component(EntityId_t entityId)
{
    ComponentId_t componentId = ComponentManager::get_component_id<T>();

	if (componentPools.at(componentId) == nullptr)
	{
		componentPools[componentId] = new ComponentPool<T>();
	}

	ComponentPool<T>& componentPool = this->get_component_pool<T>();

	componentMasks.at(entityId).set(componentId, true);

	componentPool.set(entityId);
	return componentPool.at(entityId);
}

template<ComponentData... TArgs>
bool ComponentManager::has_components(EntityId_t entityId)
{
	ComponentMask entityComponents = this->get_component_mask(entityId);
	ComponentMask componentMask = ComponentMask::from_components<TArgs...>();
	return (entityComponents.get_mask() & componentMask.get_mask()) == componentMask.get_mask();
}