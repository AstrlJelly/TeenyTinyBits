#include "game_window.hpp"
#include "GLFW/glfw3.h"
#include "scene.hpp"
#include "input_manager.hpp"
#include "shader.hpp"
#include <iostream>
#include <stb/stb_image.h>

GameWindow::GameWindow(glm::vec2 size, const char* title)
{
	if (!s_glfwInitialized)
	{
		init_glfw();
	}

	this->window = glfwCreateWindow(size.x, size.y, title, NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
        throw;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(window, on_framebuffer_size_glfw);

	if (!s_gladInitialized)
	{
		init_glad();
	}

    this->scene = new Scene();
    this->inputManager = new InputManager(window);
}

void GameWindow::init_glfw()
{
	int32_t result = glfwInit();
	if (!result)
	{
		std::cerr << "Failed to initialized GLFW" << std::endl;
		exit(1);
	}
			
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GameWindow::s_glfwInitialized = true;
}

void GameWindow::init_glad()
{
	int32_t result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!result)
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		exit(1);
	}

	glEnable(GL_DEPTH_TEST);

	GameWindow::s_gladInitialized = true;
}

GameWindow::~GameWindow()
{
    delete inputManager;
}

void GameWindow::on_framebuffer_size_glfw(GLFWwindow* window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
}

void GameWindow::start_game_loop()
{
	// IMGUI_CHECKVERSION();
	// ImGui::CreateContext();
	// ImGuiIO& io = ImGui::GetIO();
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // ImGui_ImplGlfw_InitForOpenGL(window, true);
	// ImGui_ImplOpenGL3_Init();

    // cube :]
	float vertices[] = {
		 // positions         // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	};

    // first, configure the cube's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint objectsBuffer;
	glCreateBuffers(1, &objectsBuffer);
	glNamedBufferStorage(objectsBuffer, sizeof(PhysicsObject) * TEMP_MAX_OBJECTS, nullptr, GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, objectsBuffer);
	
	PipelineShader shader = PipelineShader("shaders/shader.vert", "shaders/shader.frag");
	ComputeShader velocityCompute = ComputeShader("shaders/velocity.comp");
	ComputeShader positionCompute = ComputeShader("shaders/position.comp");

	stbi_set_flip_vertically_on_load(true);

	// /// texture stuff
	// uint diffuseMap;
	// glGenTextures(1, &diffuseMap);
	// glBindTexture(GL_TEXTURE_2D, diffuseMap);
	// // set the texture wrapping/filtering options (on the currently bound texture object)
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// // load and generate the texture
	// int32_t width, height, nrChannels;
	// unsigned char *diffuseMapData = stbi_load("assets/container2.png", &width, &height, &nrChannels, 0);
	// if (diffuseMapData)
	// {
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, diffuseMapData);
	// 	glGenerateMipmap(GL_TEXTURE_2D);
	// }
	// else
	// {
	// 	std::cout << "Failed to load texture" << std::endl;
	// }
	// stbi_image_free(diffuseMapData);

	glm::vec3 cameraPos(0.0, 0.0, 0.0);
	float zoomLevel = 1;
	const float minZoomLevel = 0.001, maxZoomLevel = 100;

	int32_t objectCount = 4;
	std::vector<PhysicsObject> objectsToAdd{};

	while (!glfwWindowShouldClose(window))
	{
		// hard code this for now
		this->initialize_frame();
		inputManager->initialize_frame(window, deltaTime);
        if (inputManager->is_key_pressed_this_frame(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, true);
        }

		glfwSwapBuffers(window);
		glfwPollEvents();

		// /// initialization stuff

		// // ImGui_ImplOpenGL3_NewFrame();
		// // ImGui_ImplGlfw_NewFrame();
		// // ImGui::NewFrame();

		// float deltaTimeF = deltaTime;

		// glm::vec2 cursorDelta = inputManager->get_cursor_delta();
		// glm::vec2 focusedCursorDelta = this->get_cursor_delta_if_focused();
		// glm::vec2 relativeCursorDelta = this->get_relative_cursor_delta();
		// glm::vec2 scrollDelta = inputManager->get_scroll_delta();

		// glm::vec2 windowSize = this->get_window_size();
		// // glm::vec2 windowPos = this->get_window_pos();

		// // float zoomLevelLog = log(zoomLevel);
		
		// glm::mat4 view = glm::mat4(1.0f);
		
		// glm::vec2 zoomScale = glm::vec2(zoomLevel);
		// view = glm::scale(view, glm::vec3(zoomScale, 1));

		// view = glm::translate(view, cameraPos);

		// glm::vec2 windowScale = glm::vec2(windowSize.y, windowSize.x);
		// if (windowSize.x < windowSize.y)
		// {
		// 	windowScale /= glm::vec2(windowSize.y);
		// }
		// else // if x is greater or equal to y
		// {
		// 	windowScale /= glm::vec2(windowSize.x);
		// }
		// view = glm::scale(view, glm::vec3(windowScale, 1));

		// glm::mat4 model = glm::mat4(1.0f);

		// /// end init stuff

		// /// input stuff

        // if (inputManager->is_mouse_button_pressed(GLFW_MOUSE_BUTTON_1)) {
		// 	glm::vec3 moveCam = glm::vec3(relativeCursorDelta.x, -relativeCursorDelta.y, 0.0);
		// 	moveCam /= zoomLevel;
		// 	cameraPos += moveCam;
        // }
		// if (inputManager->is_key_pressed_this_frame(GLFW_KEY_TAB, GLFW_MOD_SHIFT))
		// {
		// 	std::cout << "cameraPos = " << glm::to_string(cameraPos) << "\n";
		// 	std::cout << "windowScale = " << glm::to_string(windowScale) << "\n";
        // } 
		// else if (inputManager->is_key_pressed_this_frame(GLFW_KEY_TAB))
		// {
		// 	PhysicsObject* data = new PhysicsObject[TEMP_MAX_OBJECTS]();
		// 	glGetNamedBufferSubData(objectsBuffer, 0, sizeof(PhysicsObject) * objectCount, data);
		// 	for (int32_t i = 0; i < objectCount; i++) {
		// 		PhysicsObject object = data[i];
		// 		std::cout << i << " : object.position : " << glm::to_string(object.position) << "\n";
		// 	}
		// 	std::cout << std::endl;
		// }
        // if (inputManager->is_key_pressed_this_frame(GLFW_KEY_T))
        // {
		// 	GLint polygonMode;
		// 	glGetIntegerv(GL_POLYGON_MODE, &polygonMode);

		// 	if (polygonMode == GL_FILL)
		// 	{
		// 		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// 	}
		// 	else
		// 	{
		// 		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// 	}
        // }

        // if (inputManager->is_key_pressed_this_frame(GLFW_KEY_ESCAPE)) {
        //     glfwSetWindowShouldClose(window, true);
        // }
        // if (inputManager->is_key_pressed_this_frame(GLFW_KEY_F)) {
		// 	int32_t newMode = (get_cursor_mode() == GLFW_CURSOR_DISABLED) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
		// 	this->set_cursor_mode(newMode);
        // }
		// if (inputManager->is_key_pressed_this_frame(GLFW_KEY_E) || inputManager->is_key_pressed(GLFW_KEY_R)) {
		// 	objectCount++;
		// 	objectsToAdd.push_back(PhysicsObject(glm::vec2(0), glm::vec2(0), 1));
		// 	std::cout << "object count : " << objectCount << "\n";
		// 	glNamedBufferSubData(objectsBuffer, sizeof(PhysicsObject) * objectCount, sizeof(PhysicsObject) * objectsToAdd.size(), objectsToAdd.data());
		// }

		// // creates an effect where zooming gets stronger the more you zoom in or out
		// zoomLevel += (zoomLevel * -scrollDelta.y) / 10;

		// /// end input

		// /// logic
		// velocityCompute.dispatch(objectCount, 1, 1);
		// glFinish();
		// positionCompute.dispatch(objectCount, 1, 1);
		// glFinish();

		// /// end logic

		// /// rendering

		// // ImGui::ShowDemoWindow();

		// shader.use();

		// shader.set_mat4("view", view);
		// shader.set_mat4("model", model);

		// glBindVertexArray(VAO);
		// glDrawArraysInstanced(GL_TRIANGLES, 0, 6, objectCount);

		// // not technically necessary but it can make things more consistent
		// glBindVertexArray(0);

		// // ImGui::EndFrame();

		// // ImGui::Render();
		// // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// /// end rendering

		// /// more logic
		// objectsToAdd.resize(0);
		// ///
	}
}

void GameWindow::initialize_frame()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double time = glfwGetTime();
	deltaTime = time - lastFrameTime;
	lastFrameTime = time;

	// ImGui_ImplOpenGL3_NewFrame();
	// ImGui::NewFrame();
}


GameWindow* GameWindow::get_game_window(GLFWwindow* window)
{
    void* possibleGameWindow = glfwGetWindowUserPointer(window);
	// scary!!
	// shouldn't ever be called without it being valid, though.
	// i'll only use GLFWwindow in the context of GameWindow
    GameWindow* gameWindow = static_cast<GameWindow*>(possibleGameWindow);
	return gameWindow;
}


GLFWwindow* GameWindow::get_window()
{
    return window;
}
Scene* GameWindow::get_scene()
{
	return scene;
}
InputManager* GameWindow::get_input_manager()
{
	return inputManager;
}


double GameWindow::get_delta_time()
{
	return deltaTime;
}


glm::vec2 GameWindow::get_window_size()
{
	glm::vec<2, int32_t> windowSize{};
	glfwGetWindowSize(window, &windowSize.x, &windowSize.y);
	return static_cast<glm::vec2>(windowSize);
}
glm::vec2 GameWindow::get_window_pos()
{
	glm::vec<2, int32_t> windowPos{};
	glfwGetWindowPos(window, &windowPos.x, &windowPos.y);
	return static_cast<glm::vec2>(windowPos);
}

int32_t GameWindow::get_cursor_mode()
{
	return glfwGetInputMode(window, GLFW_CURSOR);
}
void GameWindow::set_cursor_mode(int32_t mode)
{
    glfwSetInputMode(window, GLFW_CURSOR, mode);
	
	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, (mode == GLFW_CURSOR_DISABLED));
	}
}

glm::vec2 GameWindow::get_cursor_delta_if_focused()
{
	if (get_cursor_mode() == GLFW_CURSOR_DISABLED)
	{
    	return inputManager->get_cursor_delta();
	}
	else
	{
		return glm::vec2(0, 0);
	}
}
glm::vec2 GameWindow::get_relative_cursor_delta()
{
	glm::vec<2, int32_t> windowSize{};
	glfwGetWindowSize(window, &windowSize.x, &windowSize.y);
	glm::vec2 windowSizeF = windowSize;
	return (inputManager->get_cursor_delta() * glm::vec2(2)) / windowSizeF;
}