#pragma once

#include "lua.hpp"
#include "LuaBridge.h"

#include "ComponentDB.h"

class Event
{
public:
	// TODO: What if a function is subscribed twice
	static inline std::unordered_map<std::string, std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>>> functions_subscribed;
	static inline std::unordered_map<std::string, std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>>> functions_subscribed_loop;

	static void Publish(std::string event_type, luabridge::LuaRef event_object = luabridge::LuaRef(ComponentDB::lua_state));
	static void Subscribe(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function);
	static void Unsubscribe(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function);

	// Should I even use this name?
	static void LateUpdate();
};

