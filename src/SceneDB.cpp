#include "SceneDB.h"

#include "Config.h"

#include "SDL/include/SDL.h"
#include "glm/glm.hpp"

Actor* SceneDB::Find(const std::string& name)
{
	for (std::shared_ptr<Actor> actor : actors)
		if (actor->actor_name == name)
			return actor.get();

	/*if (!actors_find[name].empty())
		return actors_find[name].front().get();*/

	return nullptr;
}

luabridge::LuaRef SceneDB::FindAll(const std::string& name)
{
	luabridge::LuaRef new_table = luabridge::newTable(ComponentDB::lua_state);

	// Lua is 1-indexing
	int counter = 1;

	for (std::shared_ptr<Actor> actor : actors)
		if (actor->actor_name == name)
			new_table[counter++] = actor.get();

	/*if (!actors_find[name].empty())
		for (std::shared_ptr<Actor> actor : actors_find[name])
			new_table[counter++] = actor.get();*/

	return new_table;
}

Actor* SceneDB::Instantiate(const std::string& actor_template_name)
{
	rapidjson::Document document;
	document.SetObject();

	rapidjson::Value name;
	name.SetString(actor_template_name.c_str(), document.GetAllocator());

	document.AddMember("template", name, document.GetAllocator());

	const rapidjson::Value& constRef = document;

	// Make sure that Find and FindAll works
	actors.emplace_back(std::make_shared<Actor>(constRef));
	// And it will start to execute next frame
	actors_to_add.emplace_back(actors.back());

	return actors.back().get();
}

void SceneDB::ProcessAddedActors()
{
	// Already set actors_loop = actors in ProcessDestroyedActors

	actors_to_add_loop = actors_to_add;
	actors_to_add.clear();

	for (std::shared_ptr<Actor> actor : actors_to_add_loop)
		actor->OnStart();
}

void SceneDB::Destroy(Actor* actor)
{
	actor->destroyed = true;

	//std::cout << "At frame: " << Helper::GetFrameNumber() << ", " << actor->actor_name << " is destroyed" << std::endl;

	// TODO: Well, actually we know uid is strictly increasing... (std::equal_range)
	auto it = std::find_if(actors.begin(), actors.end(), [actor](const std::shared_ptr<Actor>& ptr) {
		return ptr.get() == actor;
	});

	// Corner case: if the Destroyed actor is just added
	auto it2 = std::find_if(actors_to_add.begin(), actors_to_add.end(), [actor](const std::shared_ptr<Actor>& ptr) {
		return ptr.get() == actor;
	});

	if (it != actors.end())
		actors.erase(it);

	if (it2 != actors_to_add.end())
		actors_to_add.erase(it2);

	// It seems that OnDestroy is handled by .Lua itself
	actors_to_destroy.emplace_back(actor);
}

void SceneDB::ProcessDestroyedActors()
{
	// Justification: Find by actors is O(n) anyways, O(nm)
	actors_loop = actors;

	// OnDestroy (the only reason its here) + Its order

	/*for (Actor* actor_to_destroy : actors_to_destroy)
		actor_to_destroy->OnDestroy();*/

	actors_to_destroy.clear();
}

// Note: actor status can change inside function
void SceneDB::OnUpdate()
{
	for (std::shared_ptr<Actor> actor : actors_loop)
		actor->OnUpdate();
}

void SceneDB::OnLateUpdate()
{
	for (std::shared_ptr<Actor> actor : actors_loop)
		actor->OnLateUpdate();
}

void SceneDB::LoadScene()
{
	// Will mark all actors as newly added, so OnStart will be called in the first loop

	if (!load_scene_required)
		return;

	load_scene_required = false;

	Clear();

	// Find the scene under scene_name and read
	rapidjson::Document scene;
	std::string scene_path = "resources/scenes/" + scene_name + ".scene";

	if (std::filesystem::exists(scene_path))
	{
		EngineUtils::ReadJsonFile(scene_path, scene);
	}
	else 
	{
		std::cout << "error: scene " << scene_name << " is missing";
		exit(0);
	}

	// Get scene_actors
	const rapidjson::Value& scene_actors = scene["actors"];
	actors.reserve(scene_actors.Size());

	// actors_to_add does NOT include actors dont_destroy
	for (const rapidjson::Value& actor : scene_actors.GetArray())
	{
		actors.emplace_back(std::make_shared<Actor>(actor));
		actors_to_add.emplace_back(actors.back());
	}

	// Initialize so that it can be used
	actors_loop = actors;

	/*for (std::shared_ptr<Actor> actor : actors)
		actors_find[actor->actor_name].emplace_back(actor);*/
}

void SceneDB::Load(const std::string& scene_name_new)
{
	scene_name = scene_name_new;
	load_scene_required = true;
}

std::string SceneDB::GetCurrent()
{
	return scene_name;
}

void SceneDB::DontDestroy(Actor* actor)
{
	actor->dont_destroy = true;
}

void SceneDB::Clear()
{
	// A complicated way of saying only remove dont_destroy = false actors
	actors.erase(std::remove_if(actors.begin(), actors.end(), [](std::shared_ptr<Actor> actor) { return !actor->dont_destroy; }), actors.end());
	actors_loop.clear();

	actors_to_add.clear();
	actors_to_add_loop.clear();
	actors_to_destroy.clear();
}

