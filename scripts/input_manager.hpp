#pragma once

#include <cstdint>
#include <map>

#include "GLFW/glfw3.h"
#include "glm/glm/ext/vector_float2.hpp"

typedef uint32_t KeyState;
constexpr KeyState KEY_RELEASED = 1 << 0;
constexpr KeyState KEY_PRESSED  = 1 << 1;

class KeyStateInfo
{
    KeyState state;
    int mods;
    double timestamp;

public:
    KeyStateInfo(KeyState state = KEY_RELEASED, int mods = 0);

    void update_timestamp();
    void set_timestamp(double time);
    double get_timestamp();

    bool is_in_state(KeyState state);
    KeyState get_state();
};

class InputManager
{
    // key state (GLFW_KEY_A), keystate (duh)
    std::map<int, KeyStateInfo> allKeyStates;

    double frameStartTime;
    double lastFrameStartTime;
    bool mouseFocused;

    glm::vec2 mouseDelta;
    glm::vec2 lastMousePos;
    glm::vec2 scrollDelta;
    // shouldn't really be used, it's highly likely that this changes throughout the frame
    glm::vec2 realTimeScrollDelta;

public:
    InputManager();
    KeyStateInfo operator[](int key);

    bool is_key_pressed_this_frame(int key);
    bool is_key_pressed(int key);
    bool is_key_released_this_frame(int key);
    bool is_key_released(int key);

    glm::vec2 get_mouse_delta();
    glm::vec2 get_scroll_delta();

    void initialize_frame(GLFWwindow* window, double deltaTime);

    void on_key_glfw(GLFWwindow *window, int key, int scancode, int action, int mods);
    void on_scroll_glfw(GLFWwindow *window, double xoffset, double yoffset);
};