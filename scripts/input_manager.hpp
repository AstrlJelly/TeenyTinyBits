#pragma once

#include <cstdint>
#include <map>

#include "GLFW/glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm/gtx/string_cast.hpp"
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
    bool cursorFocused;

    // only stored for one purpose, but it's good to have it here
    glm::vec2 cursorPos;
    glm::vec2 cursorDelta;

    glm::vec2 scrollDelta;
    // // shouldn't really be used, it's highly likely that this changes throughout the frame
    glm::vec2 realTimeScrollDelta;

public:
    InputManager();
    KeyStateInfo operator[](int key);

    bool is_key_pressed_this_frame(int key);
    bool is_key_pressed(int key);
    bool is_key_released_this_frame(int key);
    bool is_key_released(int key);

    glm::vec2 get_cursor_delta();
    glm::vec2 get_scroll_delta();

    void initialize_frame(GLFWwindow* window, double deltaTime);

    void on_key_glfw(GLFWwindow *window, int key, int scancode, int action, int mods);
    void on_cursor_move_glfw(GLFWwindow *window, double x, double y);
    void on_scroll_glfw(GLFWwindow *window, double xoffset, double yoffset);
};