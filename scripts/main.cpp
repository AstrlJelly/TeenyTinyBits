#include "./main.hpp"
#include "GLFW/glfw3.h"

// need to put this in the cpp file for some reason
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

int main(void)
{
	std::cout << "Version : " << TeenyTinyBits_VERSION_MAJOR << '.'
						      << TeenyTinyBits_VERSION_MAJOR << '.'
						      << TeenyTinyBits_VERSION_PATCH << '\n';

	glfwInit();

	GameWindow* gw = new GameWindow(glm::vec2(1600, 1280), "TeenyTinyBits");

	GLFWwindow* window = gw->get_window();

	glEnable(GL_DEPTH_TEST);

	glfwSetFramebufferSizeCallback(window, on_framebuffer_size_glfw);
	glfwSetKeyCallback(window, on_key_glfw);
	glfwSetScrollCallback(window, on_scroll_glfw);
	glfwSetCursorPosCallback(window, on_cursor_move_glfw);

	gw->start_game_loop();

	glfwTerminate();
	return 0;
}

static void on_key_glfw(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    GameWindow* gw = GameWindow::get_game_window(window);
    InputManager* inputManager = gw->get_input_manager();

	inputManager->on_key_glfw(window, key, scancode, action, mods);
}

static void on_scroll_glfw(GLFWwindow *window, double xoffset, double yoffset)
{
    GameWindow* gw = GameWindow::get_game_window(window);
    InputManager* inputManager = gw->get_input_manager();

	inputManager->on_scroll_glfw(window, xoffset, yoffset);
}

static void on_cursor_move_glfw(GLFWwindow *window, double x, double y)
{
    GameWindow* gw = GameWindow::get_game_window(window);
    InputManager* inputManager = gw->get_input_manager();

	inputManager->on_cursor_move_glfw(window, x, y);
}

static void on_framebuffer_size_glfw(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}