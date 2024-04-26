#pragma once

#include <unordered_map>
#include <string>

#include "Actor.h"

#pragma warning(disable : 4996)
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

class TemplateDB
{
public:
	static inline std::unordered_map<std::string, Actor> actor_templates;

	static const Actor& AddActorTemplate(const rapidjson::Value& actor);
};

