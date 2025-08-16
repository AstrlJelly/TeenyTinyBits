#pragma once

#include <bitset>
#include <cstdint>

constexpr int32_t MAX_COMPONENTS = 32;
// can be replaced with `std::vector<bool>` if tis wished to be dynamic
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

class Scene
{
private:
    static inline int16_t s_componentCounter = 0;
    template<class T>
    // will get the id associated with the type inputted
    // stays consistent because of how templates work
    int32_t get_id();

public:
    void update(double deltaTime);

};