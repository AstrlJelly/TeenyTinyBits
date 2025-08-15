#pragma once

class ComponentSystem
{
private:
    static inline ComponentSystem* instance = nullptr;

public:
    void make_current_instance(ComponentSystem* newInstance);
    ComponentSystem* get_instance();
};