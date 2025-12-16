#pragma once

#include <cstdint>

// define EntityInt so it's obvious when a number or id is being used
typedef uint32_t EntityInt_t;
typedef EntityInt_t EntityId_t;

constexpr EntityInt_t ENTITY_START_CAPACITY = 0x0FFF;
