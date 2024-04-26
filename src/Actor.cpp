#include "Actor.h"
#include "TemplateDB.h"

std::string Actor::GetName()
{
	return actor_name;
}

int Actor::GetID()
{
	return uid;
}

luabridge::LuaRef Actor::GetComponentByKey(std::string key)
{
	if (components_ordered.find(key) != components_ordered.end())
		return *(components_ordered[key]->componentRef);

	return luabridge::LuaRef(ComponentDB::lua_state);
}

// TODO: This is fine for now as there are just 4 components in C-2 anyway
luabridge::LuaRef Actor::GetComponent(std::string type_name)
{
	for (auto& [component_name, component] : components_ordered)
		if (component->type == type_name)
			return *(component->componentRef);

	return luabridge::LuaRef(ComponentDB::lua_state);
}

luabridge::LuaRef Actor::GetComponents(std::string type_name)
{
	luabridge::LuaRef new_table = luabridge::newTable(ComponentDB::lua_state);

	// Lua is 1-indexing
	int counter = 1;

	for (auto& [component_name, component] : components_ordered)
		if (component->type == type_name)
			new_table[counter++] = *(component->componentRef);

	return new_table;
}

luabridge::LuaRef Actor::AddComponent(std::string type_name)
{
	// ComponentDB::add_component_times will be incremented inside Component(type_name)
	std::string component_key = "r" + std::to_string(ComponentDB::add_component_times);
	components_ordered[component_key] = std::make_shared<Component>(type_name);

	// As this is not ctor, it is fine to call
	InjectConvenienceReferences(*components_ordered[component_key]);

	components_just_added[component_key] = components_ordered[component_key];

	return *(components_ordered[component_key]->componentRef);
}

void Actor::ProcessAddedComponents()
{
	// To be called before Update()

	// New added components enters loop in the next frame
	for (auto& [component_key, component] : components_just_added)
	{
		components_to_loop[component_key] = component;
		// Component handles disabling automatically
		component->CallFunction("OnStart");
	}

	components_just_added.clear();
}

void Actor::RemoveComponent(luabridge::LuaRef component_ref)
{

	// State Machine
	const std::string& component_key = component_ref["key"];

	// Won't run in current Update
	component_ref["enabled"] = false;
	// It won't be found by GetComponent things
	components_ordered.erase(component_key);

	// Corner case: Add and Remove a component in the same frame
	if (components_just_added.find(component_key) != components_just_added.end())
	{
		components_just_added.erase(component_key);
		// return: it is never been added to components_to_loop
		return;
	}

	// To be removed in loop
	components_just_removed.insert(component_key);
}

void Actor::ProcessRemovedComponents()
{
	// To be called after LateUpdate()

	// Compare this with just components_to_loop = components_ordered
	for (const std::string& component_just_removed : components_just_removed)
		components_to_loop.erase(component_just_removed);

	components_just_removed.clear();
}

void Actor::InjectConvenienceReferences(Component& component)
{
	(*component.componentRef)["actor"] = this;
	// This should not be here but
	component.InitComponent();
}

Actor::Actor(const rapidjson::Value& actor, bool create_a_template)
{
	uid = create_a_template ? -1 : uid_num++;

	// Create from Tempalte
	if (!actor.HasMember("template"))
		InitActor(actor);
	else
		InitActorFromTemplate(actor);

	// Can use in ctor since std::shared_ptr<Actor>
	for (auto& [component_name, component] : components_ordered)
		InjectConvenienceReferences(*component);

	components_to_loop = components_ordered;
}

void Actor::InitActor(const rapidjson::Value& actor)
{
	// isTempate: create a template actor (instead of create from Tempalte)
	actor_name = actor.HasMember("name") ? actor["name"].GetString() : "";

	const rapidjson::Value& components_doc = actor["components"];

	for (rapidjson::Value::ConstMemberIterator itr = components_doc.MemberBegin(); itr != components_doc.MemberEnd(); ++itr)
		components_ordered[itr->name.GetString()] = std::make_shared<Component>(itr);
}

void Actor::InitActorFromTemplate(const rapidjson::Value& actor)
{
	const Actor& actor_template = TemplateDB::AddActorTemplate(actor);

	actor_name = actor.HasMember("name") ? actor["name"].GetString() : actor_template.actor_name;

	// 1. Load components from actor_template to this actor
	for (const auto& [component_name, component_template] : actor_template.components_ordered)
		components_ordered[component_name] = std::make_shared<Component>(*component_template);

	// Extra checking for Linux and MacOS
	if (!actor.HasMember("components"))
		return;

	const rapidjson::Value& components_doc = actor["components"];

	for (rapidjson::Value::ConstMemberIterator itr = components_doc.MemberBegin(); itr != components_doc.MemberEnd(); ++itr)
	{
		const std::string& component_key = itr->name.GetString();
		if (components_ordered.find(component_key) == components_ordered.end())
		{
			// 3. Load components that are not in the template (with overrides defaultly)
			components_ordered[itr->name.GetString()] = std::make_shared<Component>(itr);
		}
		else
		{
			// 2. Override variables for actor_templates
			components_ordered[component_key]->OverrideVariables(itr->value);
		}
	}
}

void Actor::OnStart()
{
	for (auto& [component_name, component] : components_to_loop)
	{
		//std::cout << component_name << std::endl;
		if (!destroyed)
			component->CallFunction("OnStart");
	}
}

void Actor::OnUpdate()
{
	for (auto& [component_name, component] : components_to_loop)
	{
		//std::cout << component_name << std::endl;
		if (!destroyed)
			component->Update();
	}
}

void Actor::OnLateUpdate()
{
	for (auto& [component_name, component] : components_to_loop)
	{
		//std::cout << component_name << std::endl;
		if (!destroyed)
			component->LateUpdate();
	}
}

void Actor::OnCollisionEnter(Collision collision)
{
	for (auto& [component_name, component] : components_to_loop)
	{
		luabridge::LuaRef componentRef(*(component->componentRef));
		luabridge::LuaRef func(componentRef["OnCollisionEnter"]);

		// TODO: enabled
		if (func.isFunction())
		{
			try
			{
				func(componentRef, collision);
			}
			catch (const luabridge::LuaException& e)
			{
				Component::ReportError(actor_name, e);
			}
		}
	}
}

void Actor::OnCollisionExit(Collision collision)
{
	for (auto& [component_name, component] : components_to_loop)
	{
		luabridge::LuaRef componentRef(*(component->componentRef));
		luabridge::LuaRef func(componentRef["OnCollisionExit"]);

		// TODO: enabled, and move to Component
		if (func.isFunction())
		{
			try
			{
				func(componentRef, collision);
			}
			catch (const luabridge::LuaException& e)
			{
				Component::ReportError(actor_name, e);
			}
		}
	}
}

void Actor::OnTriggerEnter(Collision collision)
{
	for (auto& [component_name, component] : components_to_loop)
	{
		luabridge::LuaRef componentRef(*(component->componentRef));
		luabridge::LuaRef func(componentRef["OnTriggerEnter"]);

		// TODO: enabled, and move to Component
		if (func.isFunction())
		{
			try
			{
				func(componentRef, collision);
			}
			catch (const luabridge::LuaException& e)
			{
				Component::ReportError(actor_name, e);
			}
		}
	}
}

void Actor::OnTriggerExit(Collision collision)
{
	for (auto& [component_name, component] : components_to_loop)
	{
		luabridge::LuaRef componentRef(*(component->componentRef));
		luabridge::LuaRef func(componentRef["OnTriggerExit"]);

		// TODO: enabled, and move to Component
		if (func.isFunction())
		{
			try
			{
				func(componentRef, collision);
			}
			catch (const luabridge::LuaException& e)
			{
				Component::ReportError(actor_name, e);
			}
		}
	}
}

void Actor::OnDestroy()
{
	for (auto& [component_name, component] : components_to_loop)
		component->OnDestroy();
}

Actor::~Actor()
{
	OnDestroy();
}
