#pragma once

#include <memory>

#include "lua.hpp"
#include "LuaBridge.h"

#pragma warning(disable : 4996)
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

class Component
{
public:
	// TODO: Should be removed
	Component();
	
	explicit Component(const std::string& component_type);
	explicit Component(rapidjson::Value::ConstMemberIterator itr);
	explicit Component(const Component& component_in_template);

	void OverrideVariables(const rapidjson::Value& component_json);

	bool IsEnabled();

	std::shared_ptr<luabridge::LuaRef> componentRef;
	std::string type;

	// Store convenient LuaRef to use
	std::shared_ptr<luabridge::LuaRef> update;
	std::shared_ptr<luabridge::LuaRef> late_update;
	void InitComponent();

	//bool hasStart = true;
	bool hasUpdate = false;
	bool hasLateUpdate = false;

	void CallFunction(const std::string& function_name);
	// Should I put this in EngineUtils?
	static void ReportError(const std::string& actor_name, const luabridge::LuaException& e);

	void Update();
	void LateUpdate();

	// TODO: Maybe I can get rid of this by removing components inside Actor
	bool on_destroy_called = false;
	void OnDestroy();
	~Component();
};

