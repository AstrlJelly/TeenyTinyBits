#include "game_window.hpp"

#include <cstdint>

#include "GLFW/glfw3.h"

#include "print.hpp"


void init_glfw_if_not_init()
{
	static bool s_glfwInitialized = false;

	if (!s_glfwInitialized)
	{
		int32_t result = glfwInit();
		if (!result)
		{
			teeny::println(teeny::Severity::FATAL, "Failed to initialize GLFW");
			exit(1);
		}
		
		s_glfwInitialized = true;
	}
}

void init_glad_if_not_init()
{
	static bool s_gladInitialized = false;
	
	if (!s_gladInitialized)
	{
		int32_t result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!result)
		{
			teeny::println(teeny::Severity::FATAL, "Failed to initialize GLAD");
			exit(1);
		}

		s_gladInitialized = true;
	}
}

void on_framebuffer_size_glfw(GLFWwindow* window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
}

namespace teeny
{
	void GameWindow::init()
	{
		// premature optimization? maybe.
		// is it still unlikely if you use it right? yesss... 
		if (this->window != nullptr) [[unlikely]]
		{
			teeny::println(teeny::Severity::ERROR, "Window already initialized.");
			return;
		}
	
		init_glfw_if_not_init();
	
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
		this->window = glfwCreateWindow(DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y, DEFAULT_WINDOW_TITLE, nullptr, nullptr);
		if (this->window == nullptr)
		{
			teeny::println(teeny::Severity::FATAL, "Failed to create GLFW window");
			delete this;
			throw;
		}
		glfwMakeContextCurrent(this->window);
	
		// wait until glfwMakeContextCurrent to init glad
		init_glad_if_not_init();
	
		// could have something more useful here, the user pointer is very powerful
		glfwSetWindowUserPointer(this->window, this);
		glfwSwapInterval(1);
	
		glfwSetFramebufferSizeCallback(this->window, on_framebuffer_size_glfw);
	
		glEnable(GL_DEPTH_TEST);
	}
}
