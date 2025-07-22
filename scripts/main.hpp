#pragma once

// std library
#include <fstream> // IWYU pragma: keep
#include <sstream> // IWYU pragma: keep
#include <iostream> // IWYU pragma: keep
#include <cstdio>
#include <functional>
#include <memory>

// include directories
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// project headers
#include "constants.hpp"
#include "shader.hpp"
#include "game_window.hpp"

int main(void);

void render(GLFWwindow* window);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void process_input(GameWindow* gw);
