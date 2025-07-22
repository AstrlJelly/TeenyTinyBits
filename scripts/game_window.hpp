#pragma once

#include "GLFW/glfw3.h"
#include <glm/glm/glm.hpp>
#include "glm/glm/fwd.hpp"

#include <iostream>
#include <ostream>

#include "input_manager.hpp"

class GameWindow
{
private:
    GLFWwindow* window;
    InputManager* input_manager;
    
public:
    GameWindow(glm::vec2 size, const char* title);
    static bool try_get_game_window(GLFWwindow* window, GameWindow** gw);
    static GameWindow* get_game_window(GLFWwindow* window);

    GLFWwindow* get_window();
    InputManager* get_input_manager();

    // void initialize_frame();


};