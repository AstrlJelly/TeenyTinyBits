#pragma once
#include "component_system.hpp"


template<ComponentData T>
ComponentPool<T>::ComponentPool()
{
	componentsData = std::array<T, ENTITY_START_CAPACITY>();
}

template<ComponentData T>
T& ComponentPool<T>::set(EntityId index)
{
	componentsData[index] = {};
	return this->at(index);
}

template<ComponentData T>
T& ComponentPool<T>::at(EntityInt index)
{
	return componentsData.at(index);
}

template<ComponentData T>
EntityInt ComponentPool<T>::get_size()
{
	return componentsData.size();
}