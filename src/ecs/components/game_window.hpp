#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>

#include "ecs/component.hpp"


struct GameWindow : Component
{
    GLFWwindow* window;
    double deltaTime;
    double lastFrameTime;

    inline constexpr static glm::vec2 DEFAULT_WINDOW_SIZE = glm::vec2(800, 640);
    inline constexpr static const char* DEFAULT_WINDOW_TITLE = "Teeny";

    void init();
};