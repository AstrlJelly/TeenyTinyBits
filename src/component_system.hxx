#pragma once
#include "component_system.hpp"



template<class T>
ComponentPool<T>::ComponentPool()
{
	componentsData = std::vector<T>(ENTITY_START_CAPACITY);
}

template<class T>
T ComponentPool<T>::set(EntityInt index)
{
	return componentsData[index] = T();
}

template<class T>
T ComponentPool<T>::at(EntityInt index)
{
	return componentsData[index];
}

template<class T>
EntityInt ComponentPool<T>::get_size()
{
	return componentsData.size();
}