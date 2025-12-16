#include "component_system.hpp"



ComponentManager::ComponentManager()
{
	componentPools = std::array<std::shared_ptr<IComponentPool>, MAX_COMPONENT_TYPES>();
}


ComponentMask_t ComponentManager::get_component_mask(EntityId_t entityId)
{
	return componentMasks.at(entityId);
}