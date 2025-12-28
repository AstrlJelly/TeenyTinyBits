#include "scene.hpp"

#include "component_system/component_manager.hpp"
#include "component_system/entity.hpp"
#include "component_system/entity_manager.hpp"

Scene::Scene()
{
    entityManager = EntityManager();
    componentManager = ComponentManager();
}


EntityManager& Scene::get_entity_manager()
{
    return entityManager;
}

EntityId_t Scene::new_entity()
{
    return entityManager.new_entity();
}


ComponentManager& Scene::get_component_manager()
{
    return componentManager;
}