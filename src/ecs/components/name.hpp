#pragma once

#include "ecs/component.hpp"

namespace teeny
{
    struct Name : Component
    {
        const char* name;
    };
}