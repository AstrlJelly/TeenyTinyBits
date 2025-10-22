#pragma once

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <GL/glext.h>

// #include <ImGui/imgui.h>
// #include <ImGui/backends/imgui_impl_glfw.h>
// #include <ImGui/backends/imgui_impl_opengl3.h>
#include <glm/glm/glm.hpp>

#include "scene.hpp"
#include "input_manager.hpp"

#define TEMP_MAX_OBJECTS 0xFFFF


class GameWindow
{
private:
    GLFWwindow* window;
    Scene* scene;
    InputManager* inputManager;

    double deltaTime;
    double lastFrameTime;

    static void init_glfw_if_not_init();
    static void init_glad_if_not_init();

    static void on_framebuffer_size_glfw(GLFWwindow* window, int32_t width, int32_t height);
    
public:
    GameWindow(glm::vec2 size, const char* title);
    ~GameWindow();

    /**
     * @brief Statically get the game window object from a GLFWwindow
     * 
     * @param window A GLFWwindow 
     * @return The pointer in GLFWwindow static casted to GameWindow* 
     */
    static GameWindow* get_game_window(GLFWwindow* window);

    void start_game_loop();
    void initialize_frame();

    GLFWwindow*   get_window();
    Scene*        get_scene();
    InputManager* get_input_manager();

    glm::vec2 get_window_size();
    glm::vec2 get_window_pos();

    int32_t get_cursor_mode();
    void    set_cursor_mode(int32_t mode);

    double get_delta_time();
    
    // implies that raw cursor data will be retrieved if supported
    glm::vec2 get_cursor_delta_if_focused();
    glm::vec2 get_relative_cursor_delta();
};
