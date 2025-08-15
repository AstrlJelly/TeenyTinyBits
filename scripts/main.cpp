#include "main.hpp"

// need to put this in the cpp file
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

#include "constants.hpp"
#include "game_window.hpp"

int32_t main(void)
{
	std::cout << "Version : " << TeenyTinyBits_VERSION_MAJOR << '.'
						      << TeenyTinyBits_VERSION_MINOR << '.'
						      << TeenyTinyBits_VERSION_PATCH << '\n';


	GameWindow* gw = new GameWindow(glm::vec2(1920, 1080), "TeenyTinyBits");

	gw->start_game_loop();

	glfwTerminate();
	// ImGui::DestroyContext();

	return 0;
}