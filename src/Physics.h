#pragma once

#include "Actor.h"

#include "box2d/box2d.h"

class RayCastCallback : public b2RayCastCallback
{
	float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;
};

struct HitResult
{
	Actor* actor;
	b2Vec2 point;
	b2Vec2 normal;
	bool is_trigger;

	// This is not exposed to lua, just for sorting
	float fraction;

	bool operator<(const HitResult& rhs) const
	{
		return fraction < rhs.fraction;
	};
};

class Physics
{
public:
	// In RayCastCallback maybe?
	static inline std::vector<HitResult> hit_results;

	static luabridge::LuaRef Raycast(b2Vec2 pos, b2Vec2 dir, float dist);
	static luabridge::LuaRef RaycastAll(b2Vec2 pos, b2Vec2 dir, float dist);
};

