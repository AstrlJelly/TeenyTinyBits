#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp> 

#include "ecs/component.hpp"


struct GameWindow : Component
{

    
    GLFWwindow* window;
    glm::vec3 clearColor = glm::vec3(0.165f, 0.165f, 0.2f);
    double deltaTime;
    double lastFrameTime;

    inline constexpr static glm::vec2 DEFAULT_WINDOW_SIZE = glm::vec2(1280, 720);
    inline constexpr static const char* DEFAULT_WINDOW_TITLE = "Teeny";

    void init();
};