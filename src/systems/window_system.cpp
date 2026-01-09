#include "window_system.hpp"

#include <iostream>

#include "GLFW/glfw3.h"

#include "ecs/entity.hpp"
#include "ecs/entity_component_system.hpp"
#include "components/game_window.hpp"


void WindowSystem::update(ECSManager& ecs)
{
    for (EntityId_t entityId : this->matchingEntities)
    {
        std::cout << __PRETTY_FUNCTION__ << " : " << entityId;
        GameWindow& window = ecs.get_component<GameWindow>(entityId);
        if (window.window == nullptr)
        {
            window.initialize();
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double time = glfwGetTime();
        window.deltaTime = time - window.lastFrameTime;
        window.lastFrameTime = time;
    }
}

void WindowSystem::render(ECSManager& ecs)
{
    for (EntityId_t entityId : this->matchingEntities)
    {
        GameWindow& window = ecs.get_component<GameWindow>(entityId);

		glfwSwapBuffers(window.window);
		glfwPollEvents();
    }
}