#include "component_manager.hpp"

#include <array>
#include <cstddef>

#include "ecs/component.hpp"
#include "ecs/component_pool.hpp"
#include "ecs/component_signature.hpp"
#include "ecs/entity.hpp"

namespace teeny
{
	IComponentPool& ComponentManager::get_component_pool(ComponentId_t componentId)
	{
		return *componentPools.at(componentId);
	}
	
	ComponentManager::ComponentManager()
	{
		this->componentSignatures = std::array<ComponentSignature, ENTITY_START_CAPACITY>();
		this->componentPools = std::array<IComponentPool*, MAX_COMPONENT_TYPES>();
	}
	
	ComponentManager::~ComponentManager()
	{
		for (size_t i = 0; i < componentPools.size(); i++)
		{
			delete componentPools[i];
		}
	}
	
	ComponentSignature ComponentManager::get_component_signature(EntityId_t entityId)
	{
		return componentSignatures.at(entityId);
	}
}