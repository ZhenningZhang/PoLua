#include "Config.h"

#include <filesystem>
#include <cstdlib>

#include "SceneDB.h"

void Config::LoadGameConfig()
{
	// HW4 Test Suite #0
	if (!std::filesystem::exists("resources")) {
		std::cout << "error: resources/ missing";
		exit(0);
	}

	if (!std::filesystem::exists("resources/game.config")) {
		std::cout << "error: resources/game.config missing";
		exit(0);
	}

	EngineUtils::ReadJsonFile("resources/game.config", game_config);

	// Load game title
	if (game_config.HasMember("game_title"))
		game_title = game_config["game_title"].GetString();

	// Load initial scene
	if (!game_config.HasMember("initial_scene"))
	{
		std::cout << "error: initial_scene unspecified";
		exit(0);
	}

	SceneDB::scene_name = Config::game_config["initial_scene"].GetString();
}

void Config::LoadRenderingConfig()
{
	// HW4 Test Suite #1
	if (!std::filesystem::exists("resources/rendering.config"))
		return;

	rapidjson::Document rendering_config;
	EngineUtils::ReadJsonFile("resources/rendering.config", rendering_config);

	if (rendering_config.HasMember("x_resolution"))
		x_resolution = rendering_config["x_resolution"].GetInt();

	if (rendering_config.HasMember("y_resolution"))
		y_resolution = rendering_config["y_resolution"].GetInt();

	if (rendering_config.HasMember("clear_color_r"))
		clear_color_r = rendering_config["clear_color_r"].GetInt();

	if (rendering_config.HasMember("clear_color_g"))
		clear_color_g = rendering_config["clear_color_g"].GetInt();

	if (rendering_config.HasMember("clear_color_b"))
		clear_color_b = rendering_config["clear_color_b"].GetInt();
}
