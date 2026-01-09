#pragma once

#include <cstdint>

// define EntityInt so it's obvious when a number or id is being used
using EntityInt_t = uint32_t;
// only use EntityInt_t when using a number that has to be the same size as EntityId_t
// like when defining indices in ComponentPool or returning the size of an Entity array/vector
using EntityId_t = EntityInt_t;

constexpr EntityInt_t ENTITY_START_CAPACITY = 0x8000;