#include "TemplateDB.h"
#include <filesystem>

#include "EngineUtils.h"

const Actor& TemplateDB::AddActorTemplate(const rapidjson::Value& actor)
{
	// Actor does have template attributes 
	const std::string& template_name = actor["template"].GetString();

	// When the template is not added yet, add
	if (actor_templates.find(template_name) == actor_templates.end()) 
	{
		std::string template_path = "resources/actor_templates/" + template_name + ".template";
		rapidjson::Document actor_template_doc;

		// Check template file does exist
		if (std::filesystem::exists(template_path)) {
			EngineUtils::ReadJsonFile(template_path, actor_template_doc);
		}
		else {
			std::cout << "error: template " << template_name << " is missing";
			exit(0);
		}

		// If template file does exist, add 
		actor_templates[template_name] = Actor(actor_template_doc, true);
	}

	return actor_templates[template_name];
}
