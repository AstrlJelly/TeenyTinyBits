#pragma once

// std library
#include <iostream>
#include <cstdio>

// include directories
#include <GLAD/glad.h> 
#include <GLFW/glfw3.h>

// project headers
#include <constants.hpp>

int main(void);

void render(GLFWwindow* window);

static void windowInit(GLFWwindow* window);

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
