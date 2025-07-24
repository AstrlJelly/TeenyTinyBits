#include "game_window.hpp"
#include "GLFW/glfw3.h"
#include <algorithm>
#include <cmath>
#include <ostream>

GameWindow::GameWindow(glm::vec2 size, const char* title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(size.x, size.y, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
        throw;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		exit(1);
	}

	glEnable(GL_DEPTH_TEST);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_glfw_callback);
	glfwSetKeyCallback(window, key_glfw_callback);
	glfwSetScrollCallback(window, scroll_glfw_callback);

    this->inputManager = new InputManager();
}

static void key_glfw_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    GameWindow* gw = GameWindow::get_game_window(window);
    InputManager* inputManager = gw->get_input_manager();

	inputManager->on_key_glfw(window, key, scancode, action, mods);
}

static void scroll_glfw_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    GameWindow* gw = GameWindow::get_game_window(window);
    InputManager* inputManager = gw->get_input_manager();

	inputManager->on_scroll_glfw(window, xoffset, yoffset);
}

void process_input(GameWindow* gw)
{
	// switch (action)
	// {
	// 	case GLFW_PRESS: {
	// 		switch (key) {
	// 			case GLFW_KEY_ESCAPE: {
	// 				glfwSetWindowShouldClose(window, true);
	// 			} break;
	// 			case -1: {
	// 			} break;
	// 			case GLFW_KEY_W: {
	// 				cameraPos += cameraSpeed * cameraFront;
	// 			} break;
	// 			case GLFW_KEY_S: {
	// 				cameraPos -= cameraSpeed * cameraFront;
	// 			} break;
	// 			case GLFW_KEY_A: {
	// 				cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	// 			} break;
	// 			case GLFW_KEY_D: {
	// 				cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	// 			} break;
	// 			default: break;
	// 		}
	// 	} break;
	// 	case GLFW_REPEAT: {

	// 	} break;
	// 	case GLFW_RELEASE: {

	// 	} break;
	// 	default: break;
	// }
}


void framebuffer_size_glfw_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GameWindow::~GameWindow()
{
    delete inputManager;
}

void GameWindow::start_game_loop()
{
    // cube :]
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	}; 

	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	GLuint EBO;
	glGenBuffers(1, &EBO);

	// ..:: Initialization code :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Shader shader = Shader("./shaders/shader.vert", "./shaders/shader.frag");

	stbi_set_flip_vertically_on_load(true); 

	/// texture stuff
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data1 = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data1);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char *data2 = stbi_load("assets/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load EPIC texture" << std::endl;
	}
	stbi_image_free(data2);

	std::vector<glm::vec3> cubePositions = {
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};

	glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

	double yaw   = -90.0;
	double pitch = 0.0;
	double fov   = 45.0;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window))
	{
		this->initialize_frame();
		inputManager->initialize_frame(window, deltaTime);
		float deltaTimeF = deltaTime;
		glm::vec2 mouseDelta = inputManager->get_mouse_delta();
		glm::vec2 scrollDelta = inputManager->get_scroll_delta();


		/// input stuff

        const float cameraSpeed = 0.05f; // adjust accordingly

        if (inputManager->is_key_pressed_this_frame(GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, true);
        }
        if (inputManager->is_key_pressed_this_frame(GLFW_KEY_T))
        {
            std::cout << "This should happen ONCE!!!!!\n";

			GLint polygonMode;
			glGetIntegerv(GL_POLYGON_MODE, &polygonMode);

			if (polygonMode == GL_FILL)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
        }
        if (inputManager->is_key_pressed(GLFW_KEY_Y))
        {
            std::cout << "This should happen multiple times.\n";
        }
		if (false && inputManager->is_key_pressed(GLFW_KEY_B))
		{
			double time = glfwGetTime();
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			double x = std::fmod(time * 0.2, 1.0);
			double y = (sin(time * 2) + 1) / 2;
			// glfwSetCursorPos(window, width * x, height * y);
			cubePositions.push_back(glm::vec3(x * 10, y, -10));
		}

		const float moveSpeed = 5;
		float moveAmount = deltaTimeF * moveSpeed;

		if (inputManager->is_key_pressed(GLFW_KEY_W))
		{
			cameraPos += moveAmount * cameraFront;
		}
		if (inputManager->is_key_pressed(GLFW_KEY_S))
		{
			cameraPos -= moveAmount * cameraFront;
		}
		if (inputManager->is_key_pressed(GLFW_KEY_A))
		{
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * moveAmount;
		}
		if (inputManager->is_key_pressed(GLFW_KEY_D))
		{
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * moveAmount;
		}

		const float sensitivity = 0.1f;
		yaw   += mouseDelta.x * sensitivity;
		pitch += -mouseDelta.y * sensitivity;

		pitch = std::clamp(pitch, -89.0, 89.0);

		fov += scrollDelta.y;
		fov = std::clamp(fov, 1.0, 90.0);

		/// end input

		/// rendering

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader.use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		int windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians((float)fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

		shader.set_mat4("transform", projection * view * model);
		
		shader.set_float("time", glfwGetTime());
		shader.set_int("texture1", 0);
		shader.set_int("texture2", 1);
		
		glBindVertexArray(VAO);
		for (unsigned int j = 0; j < cubePositions.size(); j++)
		{
			int i = j % cubePositions.size();
			
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.set_mat4("transform", projection * view * model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// not technically necessary but it can make things more consistent
		glBindVertexArray(0);

		/// end rendering


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void GameWindow::initialize_frame()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	deltaTime = glfwGetTime() - lastFrameTime;
	lastFrameTime = glfwGetTime();
}

GameWindow* GameWindow::get_game_window(GLFWwindow* window)
{
    void* possibleGameWindow = glfwGetWindowUserPointer(window);
    GameWindow* gameWindow = static_cast<GameWindow*>(possibleGameWindow);
	return gameWindow;
}

GLFWwindow* GameWindow::get_window()
{
    return window;
}

InputManager* GameWindow::get_input_manager()
{
    return inputManager;
}

double GameWindow::get_delta_time()
{
	return deltaTime;
}