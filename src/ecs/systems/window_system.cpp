#include "window_system.hpp"

#include <cmath>

#include "GLFW/glfw3.h"

#include "ecs/components/transform.hpp"
#include "ecs/entity.hpp"
#include "ecs/entity_component_system.hpp"
#include "ecs/components/game_window.hpp"

namespace teeny
{
    void WindowSystem::update(ECSManager& ecs)
    {
        for (EntityId_t entityId : this->matchingEntities)
        {
            GameWindow& window = ecs.get_component<GameWindow>(entityId);
            Transform& transform = ecs.get_or_add_component<Transform>(entityId);
    
            if (window.window == nullptr)
            {
                window.init();
            }
    
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
            glfwMakeContextCurrent(window.window);
    
            glm::vec3 clear = window.clearColor;
            glClearColor(clear.r, clear.g, clear.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
            glfwSwapBuffers(window.window);
            glfwPollEvents();
        }
    }
}