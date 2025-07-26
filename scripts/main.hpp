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

static void on_key_glfw(GLFWwindow *window, int key, int scancode, int action, int mods);
static void on_scroll_glfw(GLFWwindow *window, double x, double y);
static void on_cursor_move_glfw(GLFWwindow *window, double xoffset, double yoffset);
static void on_framebuffer_size_glfw(GLFWwindow* window, int width, int height);