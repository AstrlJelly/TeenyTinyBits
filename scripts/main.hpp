#pragma once

// std library
#include <fstream> // IWYU pragma: keep
#include <sstream> // IWYU pragma: keep
#include <iostream> // IWYU pragma: keep
#include <cstdio>

// include directories
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

// project headers
#include <constants.hpp>

int main(void);

void render(GLFWwindow* window);

static void window_init(GLFWwindow* window);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
