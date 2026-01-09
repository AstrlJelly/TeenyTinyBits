#pragma once

#include "ecs/component.hpp"

struct Name : Component
{
    const char* name;
};