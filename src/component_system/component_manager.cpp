#include "component_manager.hpp"


ComponentMask::ComponentMask()
{
	this->mask = {};
}

ComponentMask::ComponentBitMask_t ComponentMask::get_mask()
{
	return this->mask;
}

bool ComponentMask::at(ComponentInt_t index)
{
	return this->mask.test(index);
}
void ComponentMask::set(ComponentInt_t index, bool value)
{
	this->mask.set(index, value);
}


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

ComponentMask ComponentManager::get_component_mask(EntityId_t entityId)
{
	return componentMasks.at(entityId);
}