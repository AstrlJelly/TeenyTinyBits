#include "input_manager.hpp"
#include "GLFW/glfw3.h"
#include <bitset>
#include <iostream>
#include <ostream>

KeyStateInfo::KeyStateInfo(KeyState state, int mods)
{
    this->state = state;
    this->mods = mods;
    this->update_timestamp();
}

KeyStateInfo InputManager::operator[](int key)
{
    return allKeyStates[key];
}

void KeyStateInfo::set_timestamp(double newTime)
{
    this->timestamp = newTime;
}

void KeyStateInfo::update_timestamp()
{
    this->set_timestamp(glfwGetTime());
}

double KeyStateInfo::get_timestamp()
{
    return timestamp;
}

bool KeyStateInfo::is_in_state(KeyState stateCheck)
{
    return (this->state & stateCheck) == stateCheck;
}

KeyState KeyStateInfo::get_state()
{
    return this->state;
}

InputManager::InputManager()
{
    allKeyStates = {};
    realTimeScrollDelta = {};
}

bool InputManager::is_key_pressed_this_frame(int key)
{
    KeyStateInfo keyState = allKeyStates[key];
    return keyState.is_in_state(KEY_PRESSED) && keyState.get_timestamp() > lastFrameStartTime;
}

bool InputManager::is_key_pressed(int key)
{
    KeyStateInfo keyState = allKeyStates[key];
    return keyState.is_in_state(KEY_PRESSED);
}

bool InputManager::is_key_released_this_frame(int key)
{
    KeyStateInfo keyState = allKeyStates[key];
    return keyState.is_in_state(KEY_RELEASED) && keyState.get_timestamp() > lastFrameStartTime;
}

bool InputManager::is_key_released(int key)
{
    KeyStateInfo keyState = allKeyStates[key];
    return keyState.is_in_state(KEY_RELEASED);
}

glm::vec2 InputManager::get_mouse_delta()
{
    return mouseDelta;
}

glm::vec2 InputManager::get_scroll_delta()
{
    return scrollDelta;
}

void InputManager::initialize_frame(GLFWwindow* window, double deltaTime)
{
    lastFrameStartTime = frameStartTime;
    frameStartTime = glfwGetTime();
    bool lastMouseFocused = mouseFocused;
    mouseFocused = glfwGetWindowAttrib(window, GLFW_FOCUSED);

    glm::vec<2, double> cursorPos{};
    glfwGetCursorPos(window, &cursorPos.x, &cursorPos.y);
    // mouseDelta = static_cast<glm::vec2>(cursorPos) - lastMousePos;
    if (mouseFocused != lastMouseFocused)
    {
        lastMousePos = cursorPos;
        mouseDelta = static_cast<glm::vec2>(cursorPos) - lastMousePos;
    }
    else
    {
        mouseDelta = static_cast<glm::vec2>(cursorPos) - lastMousePos;
        lastMousePos = cursorPos;
    }
    scrollDelta = realTimeScrollDelta;
    realTimeScrollDelta = glm::vec2(0, 0);
}

void InputManager::on_key_glfw(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // handle repeating on the "user" side and in text input later
    if (action != GLFW_PRESS && action != GLFW_RELEASE) return;

    KeyState newState = action == GLFW_PRESS ? KEY_PRESSED : KEY_RELEASED;
    KeyStateInfo keyStateInfo = KeyStateInfo(newState, mods);
    allKeyStates.insert_or_assign(key, keyStateInfo);
}

void InputManager::on_scroll_glfw(GLFWwindow* window, double xoffset, double yoffset)
{
    realTimeScrollDelta += glm::vec2(xoffset, -yoffset);
}