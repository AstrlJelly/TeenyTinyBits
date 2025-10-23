#pragma once

#include <cstdint>
#include <map>

#include <glad/glad.h>
#include <memory>
#include "GLFW/glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm/glm.hpp" // IWYU pragma: keep, common header

typedef uint32_t KeyState;
constexpr KeyState KEY_RELEASED = 1 << 0;
constexpr KeyState KEY_PRESSED  = 1 << 1;

class KeyStateInfo
{
private:
    KeyState keyState;
    int32_t modState;
    double timestamp;

public:
    KeyStateInfo() {};
    static KeyStateInfo create(KeyState state = KEY_RELEASED, int32_t mods = 0);

    void update_timestamp();
    
    void   set_timestamp(double time);
    double get_timestamp();

    bool is_in_state(KeyState keyState, int32_t modState);

    bool is_in_key_state(KeyState stateCheck);
    bool is_in_mod_state(int32_t modState);
    KeyState get_key_state();
    KeyState get_mod_state();
};

class InputManager
{
private:
    // key state (GLFW_KEY_A), keystate (duh)
    std::map<int32_t, KeyStateInfo> allKeyStates;
    std::map<int32_t, KeyStateInfo> allMouseButtonStates;

    double frameStartTime;
    double lastFrameStartTime;
    bool   cursorFocused;

    // only stored for one purpose, but it's good to have it here
    glm::vec2 cursorPos;
    glm::vec2 cursorDelta;

    glm::vec2 scrollDelta;
    // shouldn't really be used, it's highly likely that this changes throughout the frame
    glm::vec2 realTimeScrollDelta;

    static void on_key_glfw_callback         (GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
    static void on_mouse_button_glfw_callback(GLFWwindow* window, int32_t button, int32_t action, int32_t mods);
    static void on_cursor_move_glfw_callback (GLFWwindow* window, double x, double y);
    static void on_scroll_glfw_callback      (GLFWwindow* window, double xoffset, double yoffset);

public:
    InputManager() {};
    static InputManager create(GLFWwindow* window);
    static std::shared_ptr<InputManager> get_input_manager(GLFWwindow* window);

    KeyStateInfo operator[](int32_t key);

    // key can be a an integer, like GLFW_KEY_A or GLFW_KEY_TAB
    // mods can be a bitfield containing GLFW_MOD_SHIFT, GLFW_MOD_CTRL, etc
    bool is_key_pressed_this_frame (int32_t key, int32_t mods = 0);
    bool is_key_pressed            (int32_t key, int32_t mods = 0);
    bool is_key_released_this_frame(int32_t key, int32_t mods = 0);
    bool is_key_released           (int32_t key, int32_t mods = 0);

    bool is_mouse_button_pressed_this_frame (int32_t button, int32_t mods = 0);
    bool is_mouse_button_pressed            (int32_t button, int32_t mods = 0);
    bool is_mouse_button_released_this_frame(int32_t button, int32_t mods = 0);
    bool is_mouse_button_released           (int32_t button, int32_t mods = 0);

    glm::vec2 get_cursor_delta();
    // implies that raw cursor data will be retrieved if supported
    // glm::vec2 get_focused_cursor_delta();
    glm::vec2 get_scroll_delta();

    void initialize_frame(GLFWwindow* window, double deltaTime);

    static void initialize_callbacks(GLFWwindow* window);

    void on_key_glfw         (GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
    void on_mouse_button_glfw(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);
    void on_cursor_move_glfw (GLFWwindow *window, double x, double y);
    void on_scroll_glfw      (GLFWwindow *window, double xoffset, double yoffset);
};