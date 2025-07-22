#include "game_window.hpp"
#include "input_manager.hpp"
#include <exception>
#include <iostream>
#include <type_traits>

GameWindow::GameWindow(glm::vec2 size, const char* title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(1600, 1280, "TeenyTinyBits", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
        throw;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
}

bool GameWindow::try_get_game_window(GLFWwindow* window, GameWindow** gw)
{
    void* possibleGameWindow = glfwGetWindowUserPointer(window);
    GameWindow* gameWindow = static_cast<GameWindow*>(possibleGameWindow);
    return gameWindow;
}

GameWindow* GameWindow::get_game_window(GLFWwindow* window)
{
    GameWindow* possibleGameWindow = nullptr;
    if (GameWindow::try_get_game_window(window, &possibleGameWindow))
    {
        return possibleGameWindow;
    }
    else
    {
        std::cerr << "Window did not have an associated GameWindow. Was the void pointer overwritten?" << std::endl;
        throw std::bad_typeid();
    }
}

GLFWwindow* GameWindow::get_window()
{
    return window;
}

InputManager* GameWindow::get_input_manager()
{
    return input_manager;
}
