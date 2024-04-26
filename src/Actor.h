#pragma once

#include <iostream>
#include <string>
#include <set>
#include <memory>

#include "glm/glm.hpp"
#include "SDL/include/SDL.h"

#include "lua.hpp"
#include "LuaBridge.h"

#pragma warning(disable : 4996)
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include "Component.h"
#include "ComponentDB.h"

#include "ContactListener.h"

class Actor
{
public:
	static inline int uid_num = 0;

	std::string actor_name;
	int uid;

	bool destroyed = false;
	bool dont_destroy = false;

	std::map<std::string, std::shared_ptr<Component>> components_ordered;
	std::map<std::string, std::shared_ptr<Component>> components_to_loop;

	std::string GetName();
	int GetID();
	
	luabridge::LuaRef GetComponentByKey(std::string key);
	luabridge::LuaRef GetComponent(std::string type_name);
	luabridge::LuaRef GetComponents(std::string type_name);

	luabridge::LuaRef AddComponent(std::string type_name);
	void ProcessAddedComponents();

	void RemoveComponent(luabridge::LuaRef component_ref);
	void ProcessRemovedComponents();

	void InjectConvenienceReferences(Component& component);

	Actor() {}
	Actor(const rapidjson::Value& actor, bool isTemplate = false);

	void InitActor(const rapidjson::Value& actor);
	void InitActorFromTemplate(const rapidjson::Value& actor);

	void OnStart();
	void OnUpdate();
	void OnLateUpdate();

	void OnCollisionEnter(Collision collision);
	void OnCollisionExit(Collision collision);

	void OnTriggerEnter(Collision collision);
	void OnTriggerExit(Collision collision);

	// TODO: Maybe I can use ~Actor
	void OnDestroy();

	~Actor();

private:
	std::map<std::string, std::shared_ptr<Component>> components_just_added;
	std::set<std::string> components_just_removed;
};