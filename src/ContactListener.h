#pragma once

#include "box2d/box2d.h"

class ContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};

// Never used this before...
class Actor;

struct Collision
{
	Actor* other;
	b2Vec2 point;
	b2Vec2 relative_velocity;
	b2Vec2 normal;
};
