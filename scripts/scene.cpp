#include "scene.hpp"

template<class T>
int32_t Scene::get_id()
{
	static int32_t s_componentId = s_componentCounter++;
	return s_componentId;
}