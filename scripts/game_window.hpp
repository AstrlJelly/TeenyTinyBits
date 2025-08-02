#pragma once

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <GL/glext.h>

// #include <ImGui/imgui.h>
// #include <ImGui/backends/imgui_impl_glfw.h>
// #include <ImGui/backends/imgui_impl_opengl3.h>
#include <stb/stb_image.h>
#include <glm/glm/glm.hpp>
#include "glm/glm/fwd.hpp"

#include <algorithm>
// #include <cmath>
#include <iostream>
#include <ostream>

#include "shader.hpp"
#include "input_manager.hpp"

#define TEMP_MAX_OBJECTS 0xFFFF


class GameWindow
{
private:
    GLFWwindow* window;
    InputManager* inputManager;

    double deltaTime;
    double lastFrameTime;

    bool glfwInitialized : 1;
    bool gladInitialized : 1;
    
public:
    GameWindow(glm::vec2 size, const char* title);
    ~GameWindow();

    static GameWindow* get_game_window(GLFWwindow* window);

    void start_game_loop();
    void initialize_frame();

    GLFWwindow* get_window();
    InputManager* get_input_manager();

    glm::vec2 get_window_size();
    glm::vec2 get_window_pos();

    int get_cursor_mode();
    void set_cursor_mode(int mode);

    double get_delta_time();
    
    // implies that raw cursor data will be retrieved if supported
    glm::vec2 get_cursor_delta_if_focused();

    glm::vec2 get_relative_cursor_delta();
};

void init_glfw();
void init_glad();
    