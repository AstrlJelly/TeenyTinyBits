#pragma once

#include "component_manager.hpp"
#include "entity_manager.hpp"

class Scene
{
private:
    EntityManager entityManager;
    ComponentManager componentManager;
public:
    Scene();


    EntityManager& get_entity_manager();

    EntityId_t new_entity();


    ComponentManager& get_component_manager();

    template<ComponentData T> static ComponentId_t get_component_id();

    template<ComponentData T> T& get_component(EntityId_t entityId);
    template<ComponentData T> T& add_component(EntityId_t entityId);
};

#include "scene.hxx"