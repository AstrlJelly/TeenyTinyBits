#pragma once
#include "scene.hpp"

template<ComponentData T>
ComponentId_t Scene::get_component_id()
{
    return ComponentManager::get_component_id<T>();
}

// i will do my best to not make this AWFULLY optimized like unity did
template<ComponentData T>
T& Scene::get_component(EntityId_t entityId)
{
    return this->componentManager.get_component<T>(entityId);
}

template<ComponentData T>
T& Scene::add_component(EntityId_t entityId)
{
    return this->componentManager.add_component<T>(entityId);
}