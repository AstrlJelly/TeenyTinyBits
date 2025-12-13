#include "scene.hpp"

#include <memory>
#include <vector>

// #include "components/transform.hpp"


Scene::Scene()
{
	allComponentPools = std::vector<std::shared_ptr<IComponentPool>>(MAX_COMPONENT_TYPES);
}

EntityId Scene::new_entity()
{
	EntityId entityId = static_cast<EntityId>(allEntities.size());
	Entity newEntity = Entity(entityId, ComponentMask());
    allEntities.push_back(newEntity);
    return entityId;
}