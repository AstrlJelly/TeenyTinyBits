#include "./main.hpp"

// need to put this in the cpp file for some reason
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

int main(void)
{
	std::cout << "Version : " << TeenyTinyBits_VERSION_MAJOR << '.'
						      << TeenyTinyBits_VERSION_MAJOR << '.'
						      << TeenyTinyBits_VERSION_PATCH << '\n';

	glfwInit();

	GameWindow* gw = new GameWindow(glm::vec2(1600, 1280), "TeenyTinyBits");
	gw->start_game_loop();

	glfwTerminate();
	return 0;
}