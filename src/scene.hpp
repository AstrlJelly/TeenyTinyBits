#pragma once

#include <memory>
#include <vector>

#include <boost/preprocessor.hpp>

#include "component_system.hpp"


class Scene
{
private:
    std::vector<Entity> allEntities;
    std::vector<std::shared_ptr<IComponentPool>> allComponentPools;

    static inline EntityInt s_componentTypeCounter = 0;

public:
    Scene();

    // will get the id associated with the type inputted
    // stays consistent because of how templates work
    template<ComponentData T> EntityInt get_component_id();

    EntityId new_entity();

    // i will do my best to not make this AWFULLY optimized like unity did
    template<ComponentData T> T& get_component(EntityId entityId);
    template<ComponentData T> T& add_component(EntityId id);
};

#include "scene.hxx"
