#include <cstdint>
#include <ios>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "GLFW/glfw3.h"

#include "ecs/scene.hpp"

using namespace teeny;

int32_t main(void)
{
	// supposedly makes std::cout faster, as long as you don't use printf
	// https://www.reddit.com/r/cpp/comments/y29qo4/comment/is1odfq
	std::ios::sync_with_stdio(false);

	// last i checked, the scene is ~537 kb
	// much too big for the heap!
	std::unique_ptr<Scene> scenePtr = std::make_unique<Scene>();
	Scene& scene = *scenePtr;

	// initialize systems

	// system init functions(?)

	// create desired entities + components


	while (scene.is_running())
	{
		scene.update();
		scene.render();
	}

	glfwTerminate();

	return 0;
}