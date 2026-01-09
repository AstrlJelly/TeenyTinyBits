// #include "game_window.hpp"

// #include <iostream>
// #include <memory>

// #include <stb/stb_image.h>
// #include <GLFW/glfw3.h>

// #include "input_manager.hpp"
// // #include "shader.hpp"
// // #include "ecs/component_manager.hpp"
// // #include "ecs/entity_manager.hpp"
// #include "components/transform.hpp"


// GameWindow::GameWindow(glm::vec2 size, const std::string& title)
// {
// 	init_glfw_if_not_init();

// 	this->window = glfwCreateWindow(size.x, size.y, title.c_str(), NULL, NULL);
// 	if (this->window == nullptr)
// 	{
// 		std::cout << "Failed to create GLFW window" << std::endl;
// 		glfwTerminate();
//         throw;
// 	}
// 	glfwMakeContextCurrent(this->window);
// 	glfwSetWindowUserPointer(this->window, this);
// 	glfwSwapInterval(1);

// 	glfwSetFramebufferSizeCallback(this->window, on_framebuffer_size_glfw);

// 	init_glad_if_not_init();

//     this->scene = std::make_unique<ECSystem>();
//     this->inputManager = std::make_unique<InputManager>(this->window);
// }

// void GameWindow::init_glfw_if_not_init()
// {
// 	static bool s_glfwInitialized = false;

// 	if (!s_glfwInitialized)
// 	{
// 		int32_t result = glfwInit();
// 		if (!result)
// 		{
// 			std::cerr << "Failed to initialized GLFW" << std::endl;
// 			exit(1);
// 		}
				
// 		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
// 		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
// 		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// 		s_glfwInitialized = true;
// 	}

// }

// void GameWindow::init_glad_if_not_init()
// {
// 	static bool s_gladInitialized = false;
// 	if (!s_gladInitialized)
// 	{
// 		int32_t result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
// 		if (!result)
// 		{
// 			std::cerr << "Failed to initialize GLAD" << std::endl;
// 			exit(1);
// 		}

// 		glEnable(GL_DEPTH_TEST);

// 		s_gladInitialized = true;
// 	}
// }

// void GameWindow::on_framebuffer_size_glfw(GLFWwindow* window, int32_t width, int32_t height)
// {
//     glViewport(0, 0, width, height);
// }

// void GameWindow::start_game_loop()
// {
// 	EntityId_t entity = scene->new_entity();
// 	Transform& transform = scene->add_component<Transform>(entity);

// 	int currentEntityIndex = 0;

// 	// shader
	
// 	while (!glfwWindowShouldClose(window))
// 	{
// 		// hard code this for now
// 		this->initialize_frame();
// 		inputManager->initialize_frame(window, deltaTime);
//         if (inputManager->is_key_pressed_this_frame(GLFW_KEY_ESCAPE)) {
//             glfwSetWindowShouldClose(window, true);
//         } 

// 		glfwSwapBuffers(window);
// 		glfwPollEvents();
// 	}
// }

// void GameWindow::initialize_frame()
// {
// 	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 	double time = glfwGetTime();
// 	deltaTime = time - lastFrameTime;
// 	lastFrameTime = time;

// 	// ImGui_ImplOpenGL3_NewFrame();
// 	// ImGui::NewFrame();
// }


// GameWindow* GameWindow::get_game_window(GLFWwindow* window)
// {
//     void* possibleGameWindow = glfwGetWindowUserPointer(window);
// 	// scary!!
// 	// shouldn't ever be called without it being valid, though.
//     GameWindow* gameWindow = static_cast<GameWindow*>(possibleGameWindow);
// 	return gameWindow;
// }


// GLFWwindow* GameWindow::get_window()
// {
//     return window;
// }
// std::unique_ptr<InputManager>& GameWindow::get_input_manager()
// {
// 	return inputManager;
// }

// std::unique_ptr<ECSystem>& GameWindow::get_scene()
// {
// 	return scene;
// }


// double GameWindow::get_delta_time()
// {
// 	return deltaTime;
// }


// glm::vec2 GameWindow::get_window_size()
// {
// 	glm::vec<2, int32_t> windowSize{};
// 	glfwGetWindowSize(window, &windowSize.x, &windowSize.y);
// 	return static_cast<glm::vec2>(windowSize);
// }
// glm::vec2 GameWindow::get_window_pos()
// {
// 	glm::vec<2, int32_t> windowPos{};
// 	glfwGetWindowPos(window, &windowPos.x, &windowPos.y);
// 	return static_cast<glm::vec2>(windowPos);
// }

// int32_t GameWindow::get_cursor_mode()
// {
// 	return glfwGetInputMode(window, GLFW_CURSOR);
// }
// void GameWindow::set_cursor_mode(int32_t mode)
// {
//     glfwSetInputMode(window, GLFW_CURSOR, mode);
	
// 	if (glfwRawMouseMotionSupported())
// 	{
// 		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, (mode == GLFW_CURSOR_DISABLED));
// 	}
// }

// glm::vec2 GameWindow::get_cursor_delta_if_focused()
// {
// 	if (get_cursor_mode() == GLFW_CURSOR_DISABLED)
// 	{
//     	return inputManager->get_cursor_delta();
// 	}
// 	else
// 	{
// 		return glm::vec2(0, 0);
// 	}
// }
// glm::vec2 GameWindow::get_relative_cursor_delta()
// {
// 	glm::vec<2, int32_t> windowSize{};
// 	glfwGetWindowSize(window, &windowSize.x, &windowSize.y);
// 	glm::vec2 windowSizeF = windowSize;
// 	return (inputManager->get_cursor_delta() * glm::vec2(2)) / windowSizeF;
// }