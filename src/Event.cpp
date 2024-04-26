#include "Event.h"

void Event::Publish(std::string event_type, luabridge::LuaRef event_object)
{
	if (!functions_subscribed_loop[event_type].empty())
	{
		for (std::pair<luabridge::LuaRef, luabridge::LuaRef>& component_function : functions_subscribed_loop[event_type])
		{
			// TODO: Try
			component_function.second(component_function.first, event_object);
		}
	}
}

void Event::Subscribe(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function)
{
	// Subscribe after LateUpdate before physics simulation step
	functions_subscribed[event_type].emplace_back(component, function);
}

void Event::Unsubscribe(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function)
{
	// Unsubscribe after LateUpdate before physics simulation step
	std::pair<luabridge::LuaRef, luabridge::LuaRef> component_function(component, function);
	auto& vec = functions_subscribed[event_type];
	vec.erase(std::remove(vec.begin(), vec.end(), component_function), vec.end());
}

void Event::LateUpdate()
{
	// TODO: easy check they are different?
	functions_subscribed_loop = functions_subscribed;
}
