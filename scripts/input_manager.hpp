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
    int modState;
    double timestamp;

public:
    KeyStateInfo(KeyState state = KEY_RELEASED, int mods = 0);

    void update_timestamp();
    void set_timestamp(double time);
    double get_timestamp();

    bool is_in_state(KeyState state);
    KeyState get_state();
    bool is_in_mod_state(int modState);
    KeyState get_mod_state();
};

class InputManager
{
    // key state (GLFW_KEY_A), keystate (duh)
    std::map<int, KeyStateInfo> allKeyStates;
    std::map<int, KeyStateInfo> allMouseButtonStates;

    double frameStartTime;
    double lastFrameStartTime;
    bool   cursorFocused;

    // only stored for one purpose, but it's good to have it here
    glm::vec2 cursorPos;
    glm::vec2 cursorDelta;

    glm::vec2 scrollDelta;
    // shouldn't really be used, it's highly likely that this changes throughout the frame
    glm::vec2 realTimeScrollDelta;

    static void on_key_glfw_callback         (GLFWwindow *window, int key, int scancode, int action, int mods);
    static void on_mouse_button_glfw_callback(GLFWwindow *window, int button, int action, int mods);
    static void on_cursor_move_glfw_callback (GLFWwindow *window, double x, double y);
    static void on_scroll_glfw_callback      (GLFWwindow *window, double xoffset, double yoffset);

public:
    InputManager();
    // defined in game_window.cpp, since including
    // it here would lead to a circular dependency
    static InputManager* get_input_manager(GLFWwindow* window);

    KeyStateInfo operator[](int key);

    // key can be a an integer, like GLFW_KEY_A or GLFW_KEY_TAB
    // mods can be a bitfield containing GLFW_MOD_SHIFT, GLFW_MOD_CTRL, etc
    bool is_key_pressed_this_frame (int key, int mods = 0);
    bool is_key_pressed            (int key, int mods = 0);
    bool is_key_released_this_frame(int key, int mods = 0);
    bool is_key_released           (int key, int mods = 0);

    bool is_mouse_button_pressed_this_frame (int button, int mods = 0);
    bool is_mouse_button_pressed            (int button, int mods = 0);
    bool is_mouse_button_released_this_frame(int button, int mods = 0);
    bool is_mouse_button_released           (int button, int mods = 0);

    glm::vec2 get_cursor_delta();
    // implies that raw cursor data will be retrieved if supported
    // glm::vec2 get_focused_cursor_delta();
    glm::vec2 get_scroll_delta();

    void initialize_frame(GLFWwindow* window, double deltaTime);

    static void initialize_callbacks(GLFWwindow* window);

    void on_key_glfw         (GLFWwindow *window, int key, int scancode, int action, int mods);
    void on_mouse_button_glfw(GLFWwindow *window, int button, int action, int mods);
    void on_cursor_move_glfw (GLFWwindow *window, double x, double y);
    void on_scroll_glfw      (GLFWwindow *window, double xoffset, double yoffset);
};