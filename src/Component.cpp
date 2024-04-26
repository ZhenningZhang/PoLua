#include "Component.h"

#include <filesystem>

#include "ComponentDB.h"
#include "Rigidbody.h"

Component::Component()
{
}

Component::Component(const std::string& component_type)
{
	// This is (exclusively) used for add components in run time

	if (component_type != "Rigidbody")
	{
		ComponentDB::LoadComponentByType(component_type);

		// Initialize componentRef to the table (named component_type)
		luabridge::LuaRef parent_table = luabridge::getGlobal(ComponentDB::lua_state, component_type.c_str());
		componentRef = std::make_shared<luabridge::LuaRef>(luabridge::newTable(ComponentDB::lua_state));
		ComponentDB::EstablishInheritance(*componentRef, parent_table);
	}
	else
	{
		Rigidbody* rigidbody = new Rigidbody();
		luabridge::LuaRef componentRefRigidbody(ComponentDB::lua_state, rigidbody);
		componentRef = std::make_shared<luabridge::LuaRef>(componentRefRigidbody);
	}

	// Assign Type
	type = component_type;

	// Add key so component can access
	(*componentRef)["key"] = "r" + std::to_string(ComponentDB::add_component_times++);
}

Component::Component(rapidjson::Value::ConstMemberIterator itr)
{
	std::string component_name = itr->name.GetString();
	std::string component_type = itr->value["type"].GetString();

	if (component_type != "Rigidbody")
	{
		ComponentDB::LoadComponentByType(component_type);

		// Initialize componentRef to the table (named component_type)
		luabridge::LuaRef parent_table = luabridge::getGlobal(ComponentDB::lua_state, component_type.c_str());
		componentRef = std::make_shared<luabridge::LuaRef>(luabridge::newTable(ComponentDB::lua_state));
		ComponentDB::EstablishInheritance(*componentRef, parent_table);

		// Override 4 types of variables
		OverrideVariables(itr->value);
	}
	else
	{
		Rigidbody* rigidbody = new Rigidbody();
		rigidbody->OverrideVariables(itr->value);
		luabridge::LuaRef componentRefRigidbody(ComponentDB::lua_state, rigidbody);
		componentRef = std::make_shared<luabridge::LuaRef>(componentRefRigidbody);
	}

	// Assign Type
	type = component_type;

	// Add key so component can access
	(*componentRef)["key"] = component_name;
}

Component::Component(const Component& component_in_template)
{
	// Since component_in_template used itr to initialized, already overrided variables
	type = component_in_template.type;

	if (type != "Rigidbody")
	{
		componentRef = std::make_shared<luabridge::LuaRef>(luabridge::newTable(ComponentDB::lua_state));
		ComponentDB::EstablishInheritance(*componentRef, (*component_in_template.componentRef));
	}
	else
	{
		// I do hope this can inherit values from rigidbody_in_template
		// Body will be created once the OnStart is called
		Rigidbody* rigidbody_in_template = component_in_template.componentRef->cast<Rigidbody*>();
		Rigidbody* rigidbody = new Rigidbody(*rigidbody_in_template);
		luabridge::LuaRef componentRefRigidbody(ComponentDB::lua_state, rigidbody);
		componentRef = std::make_shared<luabridge::LuaRef>(componentRefRigidbody);
	}
}

void Component::OverrideVariables(const rapidjson::Value& component_json)
{
	if (type == "Rigidbody")
	{
		// Since at the time I wrote it, componentRef has no idea about Rigidbody variables
		Rigidbody* rigidbody = componentRef->cast<Rigidbody*>();
		rigidbody->OverrideVariables(component_json);
		return;
	}

	// Overrides 4 types of variables (No Checking)
	for (rapidjson::Value::ConstMemberIterator itr = component_json.MemberBegin(); itr != component_json.MemberEnd(); ++itr)
	{
		const std::string& variable_name = itr->name.GetString();
		if (variable_name == "type")
			continue;

		const rapidjson::Value& value = itr->value;
		if (value.IsString())
			(*componentRef)[variable_name] = value.GetString();
		if (value.IsBool())
			(*componentRef)[variable_name] = value.GetBool();
		if (value.IsInt())
			(*componentRef)[variable_name] = value.GetInt();
		if (value.IsDouble())
			(*componentRef)[variable_name] = value.GetDouble();
	}
}

bool Component::IsEnabled()
{
	return (*componentRef)["enabled"];
}

void Component::InitComponent()
{
	// Called inside Actor::InjectConvenienceReferences

	if ((*componentRef)["OnUpdate"].isFunction())
	{
		update = std::make_shared<luabridge::LuaRef>((*componentRef)["OnUpdate"]);
		hasUpdate = true;
	}

	if ((*componentRef)["OnLateUpdate"].isFunction())
	{
		late_update = std::make_shared<luabridge::LuaRef>((*componentRef)["OnLateUpdate"]);
		hasLateUpdate = true;
	}

	(*componentRef)["enabled"] = true;
}

void Component::CallFunction(const std::string& function_name)
{
	luabridge::LuaRef func((*componentRef)[function_name]);

	if (!func.isFunction())
		return;

	if (!IsEnabled())
		return;

	try
	{
		func(*componentRef);
	}
	catch (const luabridge::LuaException& e)
	{
		const std::string& actor_name = (*componentRef)["actor"]["GetName"]((*componentRef)["actor"]);
		ReportError(actor_name, e);
	}
}

void Component::ReportError(const std::string& actor_name, const luabridge::LuaException& e)
{
	std::string error_message = e.what();
	
	// For different platforms
	std::replace(error_message.begin(), error_message.end(), '\\', '/');

	// Print the error with color
	std::cout << "\033[31m" << actor_name << " : " << error_message << "\033[0m" << std::endl;
}

void Component::Update()
{
	if (!hasUpdate)
		return;

	if (!IsEnabled())
		return;

	try
	{
		// TODO: Ask is there any performance difference
		(*update)(*componentRef);
		//(*componentRef)["OnUpdate"](*componentRef);
	}
	catch (const luabridge::LuaException& e)
	{
		const std::string& actor_name = (*componentRef)["actor"]["GetName"]((*componentRef)["actor"]);
		ReportError(actor_name, e);
	}
}

void Component::LateUpdate()
{
	if (!hasLateUpdate)
		return;

	if (!IsEnabled())
		return;

	try
	{
		(*late_update)(*componentRef);
		//(*componentRef)["OnLateUpdate"](*componentRef);
	}
	catch (const luabridge::LuaException& e)
	{
		const std::string& actor_name = (*componentRef)["actor"]["GetName"]((*componentRef)["actor"]);
		ReportError(actor_name, e);
	}
}

void Component::OnDestroy()
{
	if (on_destroy_called)
		return;
	
	on_destroy_called = true;

	// Regardless of enable property
	luabridge::LuaRef func((*componentRef)["OnDestroy"]);

	if (!func.isFunction())
		return;

	try
	{
		func(*componentRef);
	}
	catch (const luabridge::LuaException& e)
	{
		const std::string& actor_name = (*componentRef)["actor"]["GetName"]((*componentRef)["actor"]);
		ReportError(actor_name, e);
	}
}

Component::~Component()
{
	// There is an additional checking to ensure it is called only once
	OnDestroy();
}
