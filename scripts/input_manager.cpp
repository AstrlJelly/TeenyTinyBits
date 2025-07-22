#include "input_manager.hpp"

KeyState InputManager::operator[](int key)
{
    return input_map[key];
}

void KeyState::update_time(double newTime)
{
    this->timestamp = newTime;
}

void KeyState::update_time()
{
    this->update_time(glfwGetTime());
}