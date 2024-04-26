#pragma once

#include <unordered_set>

#include "lua.hpp"
#include "LuaBridge.h"

#include "Component.h"

class ComponentDB
{
public:
	static inline lua_State* lua_state;
	static inline int add_component_times = 0;
	static inline std::unordered_set<std::string> component_types_loaded;

	static void LoadComponentByType(const std::string& component_type);
	static void EstablishInheritance(luabridge::LuaRef& instance_table, luabridge::LuaRef& parent_table);

	// Not in use
	//static Component CreateRigidbody();
};

