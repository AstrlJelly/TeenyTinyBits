#pragma once

#include <iostream>
#include <stdio.h>

#include <GLAD/glad.h> 
#include <GLFW/glfw3.h>

#include "constants.hpp"

int main(void);

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
