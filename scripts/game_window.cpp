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

static void framebuffer_size_glfw_callback(GLFWwindow* window, int width, int height)
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
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f, 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	
	Shader lightingShader = Shader("./shaders/shader.vert", "./shaders/shader.frag");
	Shader lightCubeShader = Shader("./shaders/shader.vert", "./shaders/lighting.frag");

	// don't forget to use the corresponding shader program first (to set the uniform)
	lightingShader.use();
	lightingShader.set_vec3("objectColor", 1.0f, 0.5f, 0.31f);
	lightingShader.set_vec3("lightColor",  1.0f, 1.0f, 1.0f);

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
		glm::vec3( 0.0,  0.0,  0.0),
		glm::vec3( 2.0,  5.0, -15.0),
		glm::vec3(-1.5, -2.2, -2.5),
		glm::vec3(-3.8, -2.0, -12.3),
		glm::vec3( 2.4, -0.4, -3.5),
		glm::vec3(-1.7,  3.0, -7.5),
		glm::vec3( 1.3, -2.0, -2.5),
		glm::vec3( 1.5,  2.0, -2.5),
		glm::vec3( 1.5,  0.2, -1.5),
		glm::vec3(-1.3,  1.0, -1.5)
	};

	glm::vec3 lightPos   (1.2, 1.0,  2.0);

	glm::vec3 cameraPos   (0.0, 0.0,  3.0);
	glm::vec3 cameraFront (0.0, 0.0, -1.0);
	glm::vec3 cameraUp    (0.0, 1.0,  0.0);

	double yaw   = -90.0;
	double pitch =   0.0;
	double fov   =  45.0;


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

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);

		/// end input

		/// rendering

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		int windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians((float)fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

		glm::mat4 model = glm::mat4(1.0f);

		lightingShader.use();

		// lightingShader.set_mat4("projection", projection);
		// lightingShader.set_mat4("view", view);
		// lightingShader.set_mat4("model", model);
		lightingShader.set_mat4("relativeModel", projection * view * model);

		// draw the cube object
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightCubeShader.use();
		// draw the light cube object
		glBindVertexArray(lightCubeVAO);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); 

		// lightCubeShader.set_mat4("projection", projection);
		// lightCubeShader.set_mat4("view", view);
		// lightCubeShader.set_mat4("model", model);
		lightCubeShader.set_mat4("relativeModel", projection * view * model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, texture1);
		// glActiveTexture(GL_TEXTURE1);
		// glBindTexture(GL_TEXTURE_2D, texture2);
		
		// glBindVertexArray(VAO);
		// for (unsigned int j = 0; j < cubePositions.size(); j++)
		// {
		// 	int i = j % cubePositions.size();
			
		// 	glm::mat4 model = glm::mat4(1.0f);
		// 	model = glm::translate(model, cubePositions[i]);
		// 	float angle = 20.0f * i;
		// 	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		// 	lightingShader.set_mat4("relativeModel", projection * view * model);

		// 	glDrawArrays(GL_TRIANGLES, 0, 36);
		// }

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

InputManager* GameWindow::get_input_manager()
{
    return inputManager;
}

double GameWindow::get_delta_time()
{
	return deltaTime;
}