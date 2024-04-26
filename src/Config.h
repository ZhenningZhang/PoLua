#pragma once

#include <iostream>
#include "EngineUtils.h"
#include "SDL.h"

#pragma warning(disable : 4996)
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

class Config
{
public:
	// Game Config(s)
	static inline std::string game_title = "";

	// Render Config(s)
	static inline int x_resolution = 640;
	static inline int y_resolution = 360;

	static inline int clear_color_r = 255;
	static inline int clear_color_g = 255;
	static inline int clear_color_b = 255;
	static inline int clear_color_a = 255;

	static inline rapidjson::Document game_config;

	Config()
	{
		LoadGameConfig();
		LoadRenderingConfig();
	}

private:
	static void LoadGameConfig();
	static void LoadRenderingConfig();
};

