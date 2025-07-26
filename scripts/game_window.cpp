#include "game_window.hpp"
#include "GLFW/glfw3.h"
#include "glm/glm/ext/matrix_clip_space.hpp"
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/fwd.hpp"
#include "glm/glm/geometric.hpp"
#include "glm/glm/matrix.hpp"
#include "input_manager.hpp"
#include "main.hpp"
#include "shader.hpp"
#include <algorithm>
#include <cmath>
#include <ostream>

GameWindow::GameWindow(glm::vec2 size, const char* title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

	glfwSwapInterval(1);

    this->inputManager = new InputManager();
}

GameWindow::~GameWindow()
{
    delete inputManager;
}

void GameWindow::start_game_loop()
{
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
    unsigned int VBO, VAO;
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

	
	Shader lightingShader = Shader("./shaders/shader.vert", "./shaders/shader.frag");
	// ComputeShader computeShader = ComputeShader("./shaders/bits.comp");

	stbi_set_flip_vertically_on_load(true);

	/// texture stuff
	unsigned int diffuseMap;
	glGenTextures(1, &diffuseMap);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *diffuseMapData = stbi_load("./assets/container2.png", &width, &height, &nrChannels, 0);
	if (diffuseMapData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, diffuseMapData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(diffuseMapData);

	glm::vec3 cameraPos(0.0, 0.0, 0.0);
	float zoomLevel = 1;
	const float minZoomLevel = 0.001, maxZoomLevel = 100;

	while (!glfwWindowShouldClose(window))
	{
		/// initialization stuff
		this->initialize_frame();
		inputManager->initialize_frame(window, deltaTime);
		float deltaTimeF = deltaTime;
		glm::vec2 cursorDelta = inputManager->get_cursor_delta();
		glm::vec2 focusedCursorDelta = this->get_focused_cursor_delta();
		glm::vec2 relativeCursorDelta = this->get_relative_cursor_delta();
		glm::vec2 scrollDelta = inputManager->get_scroll_delta();

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, cameraPos);
		view = glm::scale(view, glm::vec3(zoomLevel, zoomLevel, 1));

		glm::mat4 model = glm::mat4(1.0f);

		/// end init stuff

		/// input stuff

        const float cameraSpeed = 0.05f; // adjust accordingly

        if (inputManager->is_mouse_button_pressed(GLFW_MOUSE_BUTTON_1)) {
			glm::vec3 moveCam = glm::vec3(relativeCursorDelta.x, -relativeCursorDelta.y, 0.0);
			cameraPos += moveCam;
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

        if (inputManager->is_key_pressed_this_frame(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, true);
        }
        if (inputManager->is_key_pressed_this_frame(GLFW_KEY_TAB)) {
			std::cout << "cameraPos = " << glm::to_string(cameraPos) << "\n";
        }
        if (inputManager->is_key_pressed_this_frame(GLFW_KEY_F)) {
			this->set_cursor_mode(get_cursor_mode() == GLFW_CURSOR_DISABLED ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }

		zoomLevel += (zoomLevel * (-scrollDelta.y / 10.0));
		zoomLevel = std::clamp(zoomLevel, minZoomLevel, maxZoomLevel);

		/// end input

		/// rendering

		lightingShader.use();

		// lightingShader.set_mat4("ortho", ortho);
		lightingShader.set_mat4("view", view);
		lightingShader.set_mat4("model", model);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

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

	double time = glfwGetTime();
	deltaTime = time - lastFrameTime;
	lastFrameTime = time;
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

glm::vec2 GameWindow::get_focused_cursor_delta()
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
	glm::vec<2, int> windowSize{};
	glfwGetWindowSize(window, &windowSize.x, &windowSize.y);
	glm::vec2 windowSizeF = windowSize;
	return (inputManager->get_cursor_delta() * glm::vec2(2)) / windowSizeF;
}

int GameWindow::get_cursor_mode()
{
	return glfwGetInputMode(window, GLFW_CURSOR);
}

void GameWindow::set_cursor_mode(int mode)
{
    glfwSetInputMode(window, GLFW_CURSOR, mode);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, (mode == GLFW_CURSOR_DISABLED));
}

InputManager* InputManager::get_input_manager(GLFWwindow* window)
{
	GameWindow* gw = GameWindow::get_game_window(window);
	return gw->get_input_manager();
}