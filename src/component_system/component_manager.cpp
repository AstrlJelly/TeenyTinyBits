#include "component_manager.hpp"



ComponentManager::ComponentManager()
{
	componentPools = std::array<IComponentPool*, MAX_COMPONENT_TYPES>();
}

ComponentManager::~ComponentManager()
{
	for (size_t i = 0; i < componentPools.size(); i++)
	{
		delete componentPools[i];
	}
}

ComponentMask_t ComponentManager::get_component_mask(EntityId_t entityId)
{
	return componentMasks.at(entityId);
}