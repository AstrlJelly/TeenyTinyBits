#pragma once

#include <map>

#include "GLFW/glfw3.h"

class KeyState
{
    int state;
    int mods;
    double timestamp;

public:
    void update_time();
    void update_time(double time);
};

class InputManager
{
    // key state (GLFW_KEY_A), keystate (duh)
    std::map<int, KeyState> input_map;

public:
    KeyState operator[](int key);

    void input_glfw_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};