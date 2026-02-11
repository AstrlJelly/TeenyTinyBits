#include "scene.hpp"
#include "GLFW/glfw3.h"
#include "ecs/system_manager.hpp"
#include "glad/glad.h"
#include "glm/glm/ext/vector_float3.hpp"
#include <cstdint>
#include <cstdlib>

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

void on_framebuffer_size_glfw(GLFWwindow* _, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
}

namespace teeny
{
    Scene::Scene()
    {
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

    bool Scene::is_running()
    {
        return this->running;
    }
    
    void Scene::update()
    {
        if (glfwWindowShouldClose(this->window))
        {
            this->lazy_exit();
        }

        double time = glfwGetTime();
        this->deltaTime = time - this->lastFrameTime;
        this->lastFrameTime = time;
    }
    
    void Scene::render()
    {
        glfwMakeContextCurrent(this->window);

        glm::vec3 clear = this->clearColor;
        glClearColor(clear.r, clear.g, clear.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }

    void Scene::lazy_exit()
    {
        this->running = false;
        teeny::println(Severity::IMPORTANT, "Exiting at end of current ECS iteration");
    }

}