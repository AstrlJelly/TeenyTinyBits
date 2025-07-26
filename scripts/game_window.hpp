#pragma once

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <ImGui/imgui.h>
#include <stb/stb_image.h>
#include <glm/glm/glm.hpp>
#include "glm/glm/fwd.hpp"

#include <iostream>
#include <ostream>

#include "shader.hpp"
#include "input_manager.hpp"

class GameWindow
{
private:
    GLFWwindow* window;
    InputManager* inputManager;

    double deltaTime;
    double lastFrameTime;
    
public:
    GameWindow(glm::vec2 size, const char* title);
    ~GameWindow();

    static GameWindow* get_game_window(GLFWwindow* window);

    void start_game_loop();
    void initialize_frame();

    GLFWwindow* get_window();
    InputManager* get_input_manager();

    double get_delta_time();
    
    // implies that raw cursor data will be retrieved if supported
    glm::vec2 get_focused_cursor_delta();

    int get_cursor_mode();
    void set_cursor_mode(int mode);
};