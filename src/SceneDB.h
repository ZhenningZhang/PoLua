#pragma once

#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <vector>

#include "TemplateDB.h"
#include "Actor.h"

#pragma warning(disable : 4996)
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <queue>

class SceneDB
{
public:
	// Find, FindAll will find in actors
	static inline std::vector<std::shared_ptr<Actor>> actors;
	// actors_loop is used for loops
	static inline std::vector<std::shared_ptr<Actor>> actors_loop;

	//static inline std::unordered_map<std::string, std::vector<std::shared_ptr<Actor>>> actors_find;

	static inline std::vector<std::shared_ptr<Actor>> actors_to_add;
	static inline std::vector<std::shared_ptr<Actor>> actors_to_add_loop;
	static inline std::vector<Actor*> actors_to_destroy;

	static Actor* Find(const std::string& name);
	static luabridge::LuaRef FindAll(const std::string& name);

	static Actor* Instantiate(const std::string& actor_template_name);
	static void ProcessAddedActors();

	static void Destroy(Actor* actor);
	static void ProcessDestroyedActors();

	// Actually, Can just use one function (with fn name)
	static void OnUpdate();
	static void OnLateUpdate();
	
	static inline bool load_scene_required = true;
	static inline std::string scene_name;
	static void LoadScene();

	static void Load(const std::string& scene_name_new);
	static std::string GetCurrent();

	static void DontDestroy(Actor* actor);

	// Need Config and ImageDB initialized
	SceneDB() {}

private:
	static void Clear();
};

