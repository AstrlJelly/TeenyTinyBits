#pragma once

// std library
#include <iostream> // IWYU pragma: keep

// include directories
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

// project headers
#include "constants.hpp"
#include "game_window.hpp"

int main(void);

static void on_framebuffer_size_glfw(GLFWwindow* window, int width, int height);