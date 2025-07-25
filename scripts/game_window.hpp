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
};

static void key_glfw_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void scroll_glfw_callback(GLFWwindow *window, double xoffset, double yoffset);
static void framebuffer_size_glfw_callback(GLFWwindow* window, int width, int height);