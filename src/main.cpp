// #include <iostream>

// need to put this in the cpp file
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "game_window.hpp"

int32_t main(void)
{
	// std::cout << "Version : " << TeenyTinyBits_VERSION_MAJOR << '.'
	// 					      << TeenyTinyBits_VERSION_MINOR << '.'
	// 					      << TeenyTinyBits_VERSION_PATCH << '\n';


	GameWindow gw = GameWindow(glm::vec2(1920, 1080), "TeenyTinyBits");

	gw.start_game_loop();

	glfwTerminate();
	// ImGui::DestroyContext();

	return 0;
}