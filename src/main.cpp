#include <iostream>
#include <string>
#include <thread>

#include "Engine.h"
#include "SDL/include/SDL.h"

#include "lua.hpp"
#include "LuaBridge.h"

#include "box2d/box2d.h"

void CppDebugLog(const std::string& message)
{
	std::cout << message << std::endl;
}

void CppDebugLogError(const std::string& message)
{
	std::cerr << message << std::endl;
}

int GetFrame()
{
	return Helper::GetFrameNumber();
}

void QuitGame()
{
	exit(0);
}

void Sleep(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void OpenURL(const std::string& url)
{
	std::string url_os;
#ifdef _WIN32
	url_os = "start " + url;
#elif __APPLE__
	url_os = "open " + url;
#elif __linux__
	url_os = "xdg-open " + url;
#else
	std::cerr << "Unsupported operating system." << std::endl;
#endif
	std::system(url_os.c_str());
}

// TODO: Make them more elegant
bool GetKey(std::string keycode)
{
	SDL_Scancode scancode = Input::GetScancodeFromName(keycode.c_str());
	return Input::GetKey(scancode);
}

bool GetKeyDown(std::string keycode)
{
	SDL_Scancode scancode = Input::GetScancodeFromName(keycode.c_str());
	return Input::GetKeyDown(scancode);
}

bool GetKeyUp(std::string keycode)
{
	SDL_Scancode scancode = Input::GetScancodeFromName(keycode.c_str());
	return Input::GetKeyUp(scancode);
}

//int main()
int main(int argc, char* argv[])
{
	//std::cout << "Hello game engines!" << std::endl;

	lua_State* lua_state = luaL_newstate();	
	ComponentDB::lua_state = lua_state;

	luaL_openlibs(lua_state);

	luabridge::getGlobalNamespace(lua_state)
		.beginNamespace("Debug")
		.addFunction("Log", &CppDebugLog)
		.addFunction("LogError", &CppDebugLogError)
		.endNamespace();

	luabridge::getGlobalNamespace(lua_state)
		.beginClass<Actor>("Actor")
		.addFunction("GetName", &Actor::GetName)
		.addFunction("GetID", &Actor::GetID)
		.addFunction("GetComponentByKey", &Actor::GetComponentByKey)
		.addFunction("GetComponent", &Actor::GetComponent)
		.addFunction("GetComponents", &Actor::GetComponents)
		.addFunction("AddComponent", &Actor::AddComponent)
		.addFunction("RemoveComponent", &Actor::RemoveComponent)
		.endClass();

	luabridge::getGlobalNamespace(lua_state)
		.beginNamespace("Actor")
		.addFunction("Find", &SceneDB::Find)
		.addFunction("FindAll", &SceneDB::FindAll)
		.addFunction("Instantiate", &SceneDB::Instantiate)
		.addFunction("Destroy", &SceneDB::Destroy)
		.endNamespace();

	luabridge::getGlobalNamespace(lua_state)
		.beginNamespace("Application")
		.addFunction("Quit", &QuitGame)
		.addFunction("Sleep", &Sleep)
		.addFunction("GetFrame", &GetFrame)
		.addFunction("OpenURL", &OpenURL)
		.endNamespace();

	luabridge::getGlobalNamespace(lua_state)
		.beginClass<glm::vec2>("vec2")
		.addProperty("x", &glm::vec2::x)
		.addProperty("y", &glm::vec2::y)
		.endClass();

	luabridge::getGlobalNamespace(lua_state)
		.beginNamespace("Input")
		.addFunction("GetKey", &GetKey)
		.addFunction("GetKeyDown", &GetKeyDown)
		.addFunction("GetKeyUp", &GetKeyUp)
		.addFunction("GetMousePosition", &Input::GetMousePosition)
		.addFunction("GetMouseButton", &Input::GetMouseButton)
		.addFunction("GetMouseButtonDown", &Input::GetMouseButtonDown)
		.addFunction("GetMouseButtonUp", &Input::GetMouseButtonUp)
		.addFunction("GetMouseScrollDelta", &Input::GetMouseScrollDelta)
		.endNamespace();

	luabridge::getGlobalNamespace(lua_state)
		.beginNamespace("Text")
		.addFunction("Draw", &TextDB::Draw)
		.endNamespace();

	luabridge::getGlobalNamespace(lua_state)
		.beginNamespace("Audio")
		.addFunction("Play", &AudioDB::Play)
		.addFunction("Halt", &AudioDB::Halt)
		.addFunction("SetVolume", &AudioDB::SetVolume)
		.endNamespace();

	luabridge::getGlobalNamespace(lua_state)
		.beginNamespace("Image")
		.addFunction("DrawUI", &ImageDB::DrawUI)
		.addFunction("DrawUIEx", &ImageDB::DrawUIEx)
		.addFunction("Draw", &ImageDB::Draw)
		.addFunction("DrawEx", &ImageDB::DrawEx)
		.addFunction("DrawPixel", &ImageDB::DrawPixel)
		.endNamespace();

	luabridge::getGlobalNamespace(lua_state)
		.beginNamespace("Camera")
		.addFunction("SetPosition", &Camera::SetPosition)
		.addFunction("GetPositionX", &Camera::GetPositionX)
		.addFunction("GetPositionY", &Camera::GetPositionY)
		.addFunction("SetZoom", &Camera::SetZoom)
		.addFunction("GetZoom", &Camera::GetZoom)
		.endNamespace();

	luabridge::getGlobalNamespace(lua_state)
		.beginNamespace("Scene")
		.addFunction("Load", &SceneDB::Load)
		.addFunction("GetCurrent", &SceneDB::GetCurrent)
		.addFunction("DontDestroy", &SceneDB::DontDestroy)
		.endNamespace();

	// TODO: What is void(*) (float, float) and float (*)(const b2Vec2&, const b2Vec2&)
	luabridge::getGlobalNamespace(lua_state)
		.beginClass<b2Vec2>("Vector2")
		.addConstructor<void(*) (float, float)>()
		.addProperty("x", &b2Vec2::x)
		.addProperty("y", &b2Vec2::y)
		.addFunction("Normalize", &b2Vec2::Normalize)
		.addFunction("Length", &b2Vec2::Length)
		.addFunction("__add", &b2Vec2::operator_add)
		.addFunction("__sub", &b2Vec2::operator_sub)
		.addFunction("__mul", &b2Vec2::operator_mul)
		.addStaticFunction("Distance", &b2Distance)
		.addStaticFunction("Dot", static_cast<float (*)(const b2Vec2&, const b2Vec2&)>(&b2Dot))
		.endClass();

	// For this specific C++ component (so lua can access)
	luabridge::getGlobalNamespace(lua_state)
		.beginClass<Rigidbody>("Rigidbody")

		// Component Properties
		.addData("type", &Rigidbody::type)
		.addData("key", &Rigidbody::key)
		.addData("actor", &Rigidbody::actor)
		.addData("enabled", &Rigidbody::enabled)

		.addFunction("OnStart", &Rigidbody::OnStart)
		// HW8 Test Suite #7
		.addFunction("OnDestroy", &Rigidbody::OnDestroy)

		// Getters
		.addFunction("GetPosition", &Rigidbody::GetPosition)
		.addFunction("GetRotation", &Rigidbody::GetRotation)
		.addFunction("GetWidth", &Rigidbody::GetWidth)
		.addFunction("GetHeight", &Rigidbody::GetHeight)
		.addFunction("GetVelocity", &Rigidbody::GetVelocity)
		.addFunction("GetAngularVelocity", &Rigidbody::GetAngularVelocity)
		.addFunction("GetGravityScale", &Rigidbody::GetGravityScale)
		.addFunction("GetUpDirection", &Rigidbody::GetUpDirection)
		.addFunction("GetRightDirection", &Rigidbody::GetRightDirection)

		// Setters and other functions
		.addFunction("AddForce", &Rigidbody::AddForce)
		.addFunction("SetVelocity", &Rigidbody::SetVelocity)
		.addFunction("SetAngularVelocity", &Rigidbody::SetAngularVelocity) 
		.addFunction("SetPosition", &Rigidbody::SetPosition) 
		.addFunction("SetRotation", &Rigidbody::SetRotation) 
		.addFunction("SetGravityScale", &Rigidbody::SetGravityScale) 
		.addFunction("SetUpDirection", &Rigidbody::SetUpDirection) 
		.addFunction("SetRightDirection", &Rigidbody::SetRightDirection) 

		// Just to pass the test case
		.addData("x", &Rigidbody::x)
		.addData("y", &Rigidbody::y)
		.addData("rotation", &Rigidbody::rotation)
		.addData("gravity_scale", &Rigidbody::gravity_scale)

		.endClass();

	luabridge::getGlobalNamespace(lua_state)
		.beginClass<Collision>("Collision")
		.addData("other", &Collision::other)
		.addData("point", &Collision::point)
		.addData("relative_velocity", &Collision::relative_velocity)
		.addData("normal", &Collision::normal)
		.endClass();

	luabridge::getGlobalNamespace(lua_state)
		.beginNamespace("Physics")
		.addFunction("Raycast", &Physics::Raycast)
		.addFunction("RaycastAll", &Physics::RaycastAll)
		.endNamespace();

	luabridge::getGlobalNamespace(lua_state)
		.beginClass<HitResult>("HitResult")
		.addData("actor", &HitResult::actor)
		.addData("point", &HitResult::point)
		.addData("normal", &HitResult::normal)
		.addData("is_trigger", &HitResult::is_trigger)
		.endClass();

	luabridge::getGlobalNamespace(lua_state)
		.beginNamespace("Event")
		.addFunction("Publish", &Event::Publish)
		.addFunction("Subscribe", &Event::Subscribe)
		.addFunction("Unsubscribe", &Event::Unsubscribe)
		.endNamespace();

	Engine engine;

	engine.GameLoop();
	
	return 0;
}
