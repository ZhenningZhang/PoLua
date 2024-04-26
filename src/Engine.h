#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

#include "glm/glm.hpp"
#include "SDL.h"

#pragma warning(disable : 4996)
#include "rapidjson/document.h"

#include "Config.h"
#include "SceneDB.h"
#include "TemplateDB.h"

#include "Renderer.h"
#include "ImageDB.h"
#include "TextDB.h"
#include "AudioDB.h"
#include "Input.h"
#include "Camera.h"
#include "Rigidbody.h"
#include "Physics.h"
#include "Event.h"

class Engine
{
public:
	void GameLoop();

private:
	std::string current_input;
	bool quit = false;

	// Everything needs Config
	Config config;
	Renderer renderer;
	AudioDB audioDB;

	// SceneDB needs Renderer (for ImageDB) and AudioDB
	SceneDB sceneDB;
	// ImageDB needs Renderer and SceneDB (for Hearts)
	ImageDB imageDB;
	
	// Don't think order matters
	TemplateDB templateDB;
	TextDB textDB;
	Input input;

	void Start();
};

