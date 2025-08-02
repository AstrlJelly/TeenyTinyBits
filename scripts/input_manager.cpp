#include "game_window.hpp"
#include "input_manager.hpp"

KeyStateInfo::KeyStateInfo(KeyState state, int mods)
{
    this->state = state;
    this->modState = mods;
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
bool KeyStateInfo::is_in_mod_state(int mods)
{
    return (this->modState & mods) == mods;
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

InputManager* InputManager::get_input_manager(GLFWwindow* window)
{
	GameWindow* gw = GameWindow::get_game_window(window);
	return gw->get_input_manager();
}

void InputManager::initialize_callbacks(GLFWwindow *window)
{
	glfwSetKeyCallback(window, on_key_glfw_callback);
    glfwSetMouseButtonCallback(window, on_mouse_button_glfw_callback);
	glfwSetCursorPosCallback(window, on_cursor_move_glfw_callback);
	glfwSetScrollCallback(window, on_scroll_glfw_callback);
}


bool InputManager::is_key_pressed_this_frame(int key, int mods)
{
    KeyStateInfo keyState = allKeyStates[key];
    return keyState.is_in_state(KEY_PRESSED) && keyState.get_timestamp() > lastFrameStartTime;
}
bool InputManager::is_key_pressed(int key, int mods)
{
    KeyStateInfo keyState = allKeyStates[key];
    return keyState.is_in_state(KEY_PRESSED);
}
bool InputManager::is_key_released_this_frame(int key, int mods)
{
    KeyStateInfo keyState = allKeyStates[key];
    return keyState.is_in_state(KEY_RELEASED) && keyState.get_timestamp() > lastFrameStartTime;
}
bool InputManager::is_key_released(int key, int mods)
{
    KeyStateInfo keyState = allKeyStates[key];
    return keyState.is_in_state(KEY_RELEASED);
}


bool InputManager::is_mouse_button_pressed_this_frame(int button, int mods)
{
    KeyStateInfo keyState = allMouseButtonStates[button];
    return keyState.is_in_state(KEY_PRESSED) && keyState.get_timestamp() > lastFrameStartTime;
}
bool InputManager::is_mouse_button_pressed(int button, int mods)
{
    KeyStateInfo keyState = allMouseButtonStates[button];
    return keyState.is_in_state(KEY_PRESSED);
}
bool InputManager::is_mouse_button_released_this_frame(int button, int mods)
{
    KeyStateInfo keyState = allMouseButtonStates[button];
    return keyState.is_in_state(KEY_RELEASED) && keyState.get_timestamp() > lastFrameStartTime;
}
bool InputManager::is_mouse_button_released(int button, int mods)
{
    KeyStateInfo keyState = allMouseButtonStates[button];
    return keyState.is_in_state(KEY_RELEASED);
}


glm::vec2 InputManager::get_cursor_delta()
{
    return cursorDelta;
}
glm::vec2 InputManager::get_scroll_delta()
{
    return scrollDelta;
}


void InputManager::initialize_frame(GLFWwindow* window, double deltaTime)
{
    lastFrameStartTime = frameStartTime;
    frameStartTime = glfwGetTime();
    bool lastMouseFocused = cursorFocused;
    cursorFocused = glfwGetWindowAttrib(window, GLFW_FOCUSED);

    glm::vec2 lastMousePos = cursorPos;
    glm::vec<2, double> cursorPosD{};
    glfwGetCursorPos(window, &cursorPosD.x, &cursorPosD.y);
    this->cursorPos = cursorPosD;
    
    if ((cursorFocused || lastMouseFocused) && (cursorFocused != lastMouseFocused))
    {
        cursorDelta = glm::vec2(0, 0);
    }
    else
    {
        cursorDelta = cursorPos - lastMousePos;
    }

    scrollDelta = realTimeScrollDelta;
    realTimeScrollDelta = glm::vec2(0, 0);
}


void InputManager::on_key_glfw_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    InputManager* inputManager = InputManager::get_input_manager(window);
	inputManager->on_key_glfw(window, key, scancode, action, mods);
}
void InputManager::on_mouse_button_glfw_callback(GLFWwindow *window, int button, int action, int mods)
{
    InputManager* inputManager = InputManager::get_input_manager(window);
	inputManager->on_mouse_button_glfw(window, button, action, mods);
}
void InputManager::on_scroll_glfw_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    InputManager* inputManager = InputManager::get_input_manager(window);
	inputManager->on_scroll_glfw(window, xoffset, yoffset);
}
void InputManager::on_cursor_move_glfw_callback(GLFWwindow *window, double x, double y)
{
    InputManager* inputManager = InputManager::get_input_manager(window);
	inputManager->on_cursor_move_glfw(window, x, y);
}


void InputManager::on_key_glfw(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // handle repeating on the "user" side and in text input later
    if (action != GLFW_PRESS && action != GLFW_RELEASE) return;

    KeyState newState = action == GLFW_PRESS ? KEY_PRESSED : KEY_RELEASED;
    KeyStateInfo keyStateInfo = KeyStateInfo(newState, mods);
    allKeyStates.insert_or_assign(key, keyStateInfo);
}
void InputManager::on_mouse_button_glfw(GLFWwindow* window, int button, int action, int mods)
{
    KeyState newState = action == GLFW_PRESS ? KEY_PRESSED : KEY_RELEASED;
    KeyStateInfo keyStateInfo = KeyStateInfo(newState, mods);
    allMouseButtonStates.insert_or_assign(button, keyStateInfo);
}
void InputManager::on_cursor_move_glfw(GLFWwindow* window, double x, double y)
{
    // realTimeMouseDelta += glm::vec2(x / 1000, y / 1000);
    // std::cout << "realTimeMouseDelta : " << glm::to_string(realTimeMouseDelta) << "\n";
}
void InputManager::on_scroll_glfw(GLFWwindow* window, double xoffset, double yoffset)
{
    realTimeScrollDelta += glm::vec2(xoffset, -yoffset);
}