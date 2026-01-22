#include "ecs/components/game_window.hpp"
#include "ecs/entity.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "GLFW/glfw3.h"

#include "ecs/entity_component_system.hpp"
#include "ecs/systems/window_system.hpp"

using namespace teeny;

int32_t main(void)
{
	// last i checked, the ecs manager is ~537 kb
	// much too big for the heap!
	ECSManager& ecs = *new ECSManager();

	// initialize systems
	WindowSystem& windowSystem = ecs.register_system<WindowSystem>();

	// system init functions(?)

	// create desired entities + components
	EntityId_t windowId = ecs.new_entity();
	GameWindow& gameWindow = ecs.add_component<GameWindow>(windowId);
	// EntityId_t windowId2 = ecs.new_entity();
	// ecs.add_component<GameWindow>(windowId2);

	while (ecs.is_running())
	{
		windowSystem.update(ecs);

		windowSystem.render(ecs);
	}

	delete &ecs;

	glfwTerminate();

	return 0;
}