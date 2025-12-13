#include "component_system.hpp"


Entity::Entity(EntityId id, ComponentMask mask)
{
	this->id = id;
	this->mask = mask;
}

EntityId Entity::get_id()
{
	return this->id;
}
ComponentMask Entity::get_mask()
{
	return this->mask;
}

void Entity::set_bit_in_mask(EntityInt position, bool value)
{
	this->mask.set(position, value);
}
