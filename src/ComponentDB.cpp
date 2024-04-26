#include "ComponentDB.h"

#include <filesystem>

#include "Rigidbody.h"

void ComponentDB::LoadComponentByType(const std::string& component_type)
{
	if (component_types_loaded.find(component_type) != component_types_loaded.end())
		return;

	std::string component_path = "resources/component_types/" + component_type + ".lua";

	// Check if lua file exist
	if (!std::filesystem::exists(component_path))
	{
		std::cout << "error: failed to locate component " << component_type;
		exit(0);
	}

	// Define table inside Lua file (for linking metatable)
	if (luaL_dofile(ComponentDB::lua_state, component_path.c_str()) != LUA_OK)
	{
		std::cout << "problem with lua file " << component_type;
		exit(0);
	}

	// Extra checking: component table name != component file name
	if (luabridge::getGlobal(ComponentDB::lua_state, component_type.c_str()) == luabridge::LuaRef(lua_state))
	{
		std::cout << "problem with lua file " << component_type << ": table name is different from the component type";
		exit(0);
	}

	component_types_loaded.insert(component_type);
}

void ComponentDB::EstablishInheritance(luabridge::LuaRef& instance_table, luabridge::LuaRef& parent_table)
{
	luabridge::LuaRef new_metatable = luabridge::newTable(lua_state);
	new_metatable["__index"] = parent_table;

	instance_table.push(lua_state);
	new_metatable.push(lua_state);
	lua_setmetatable(lua_state, -2);
	lua_pop(lua_state, 1);
}

//Component ComponentDB::CreateRigidbody()
//{
//	Component component;
//
//	// TODO: lua cannot use sharedpointer?
//	Rigidbody* rigidbody = new Rigidbody();
//	luabridge::LuaRef componentRef(lua_state, rigidbody);
//	component.componentRef = std::make_shared<luabridge::LuaRef>(componentRef);
//
//	component.type = "Rigidbody";
//
//	// I might not even need this
//	component.hasUpdate = false;
//	component.hasUpdate = false;
//
//	return Component();
//}
