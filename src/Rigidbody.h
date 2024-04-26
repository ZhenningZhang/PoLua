#pragma once

#include "box2d/box2d.h"

#include "Actor.h"
#include "ContactListener.h"

class Rigidbody
{
public:
	static inline b2World* world;
	static inline ContactListener contact_listener;

	// Variables to be identified as a component
	std::string type = "Rigidbody";
	// Initialized in Component(itr) ctor
	std::string key = "???";
	// Initialized in InjectConvenienceReferences
	Actor* actor = nullptr;
	bool enabled = true;

	// Variables for rigidbody ifself
	b2Body* body;

	// Variables (only) for Init
	float x = 0.0f;
	float y = 0.0f;

	std::string body_type = "dynamic";
	bool precise = true;
	float gravity_scale = 1.0f;
	float density = 1.0f;
	float angular_friction = 0.3f;
	float rotation = 0.0f;

	bool has_collider = true;
	bool has_trigger = true;

	// HW8 Test Suite #3
	std::string collider_type = "box";
	float width = 1.0f;
	float height = 1.0f;
	float radius = 0.5f;
	float friction = 0.3f;
	float bounciness = 0.3f;

	// HW8 Test Suite #4
	std::string trigger_type = "box";
	float trigger_width = 1.0f;
	float trigger_height = 1.0f;
	float trigger_radius = 0.5f;

	// A template never have the body assigned
	void OnStart();

	void OverrideVariables(const rapidjson::Value& component_json);
	void CreateRigidbody();

	// TODO: Before OnStart even called
	
	// Get
	b2Vec2 GetPosition();
	float GetRotation();

	b2Vec2 GetVelocity();

	float GetAngularVelocity();
	float GetGravityScale();

	b2Vec2 GetUpDirection();
	b2Vec2 GetRightDirection();

	// Set
	void AddForce(const b2Vec2& force);
	
	void SetVelocity(const b2Vec2& velocity);
	void SetAngularVelocity(const float degrees_clockwise);

	void SetPosition(const b2Vec2& position);
	void SetRotation(const float degrees_clockwise);

	void SetGravityScale(const float gravity_scale);

	void SetUpDirection(b2Vec2 direction);
	void SetRightDirection(b2Vec2 direction);

	void OnDestroy();
};

