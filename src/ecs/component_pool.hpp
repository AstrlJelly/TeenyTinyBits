#pragma once

#include <array>

#include "component.hpp"
#include "entity.hpp"

namespace teeny
{
    struct IComponentPool
    {
        virtual ~IComponentPool() {};
    };
    
    template<ComponentData T>
    class ComponentPool : public IComponentPool
    {
    private:
        // TODO: optimize with indices so data can be cache hit (and for lower memory usage?)
        std::array<EntityInt_t, ENTITY_START_CAPACITY> indices;
        std::array<T, ENTITY_START_CAPACITY> componentsData;
    
    public:
        ComponentPool()
        {
            indices = std::array<EntityInt_t, ENTITY_START_CAPACITY>();
            componentsData = std::array<T, ENTITY_START_CAPACITY>();
        }
    
        void set(EntityId_t idIndex, T value)
        {
            componentsData[idIndex] = value;
        }
        [[nodiscard]] T& at(EntityId_t idIndex)
        {
            return componentsData.at(idIndex);
        }
    
        [[nodiscard]] EntityInt_t get_size()
        {
            return this->componentsData.size();
        }
    };
}