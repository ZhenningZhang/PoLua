#include "ContactListener.h"

#include "Actor.h"

b2Vec2 INVALID_VEC(-999.0f, -999.0f);

void ContactListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	Actor* actorA = reinterpret_cast<Actor*>(fixtureA->GetUserData().pointer);

	b2Fixture* fixtureB = contact->GetFixtureB();
	Actor* actorB = reinterpret_cast<Actor*>(fixtureB->GetUserData().pointer);

	// TODO: This may be really bad for debugging. (It means phantom fixture)
	if (actorA == nullptr || actorB == nullptr)
		return;

	//std::cout << "Begin Contact " << actorA->actor_name << " - " << actorB->actor_name << std::endl;

	b2WorldManifold world_manifold;
	contact->GetWorldManifold(&world_manifold);

	Collision collision;
	collision.other = actorB;
	collision.point = world_manifold.points[0];
	collision.normal = world_manifold.normal;
	collision.relative_velocity = fixtureA->GetBody()->GetLinearVelocity() - fixtureB->GetBody()->GetLinearVelocity();

	if (!fixtureA->IsSensor() && !fixtureB->IsSensor())
	{
		actorA->OnCollisionEnter(collision);
		collision.other = actorA;
		actorB->OnCollisionEnter(collision);
	}

	if (fixtureA->IsSensor() && fixtureB->IsSensor())
	{
		// HW8 Test Suite #5: point and normal are both set to the sentinel value
		collision.point = INVALID_VEC;
		collision.normal = INVALID_VEC;

		actorA->OnTriggerEnter(collision);
		collision.other = actorA;
		actorB->OnTriggerEnter(collision);
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	Actor* actorA = reinterpret_cast<Actor*>(fixtureA->GetUserData().pointer);

	b2Fixture* fixtureB = contact->GetFixtureB();
	Actor* actorB = reinterpret_cast<Actor*>(fixtureB->GetUserData().pointer);

	// TODO: This may be really bad for debugging. (It means phantom fixture)
	if (actorA == nullptr || actorB == nullptr)
		return;

	b2WorldManifold world_manifold;
	contact->GetWorldManifold(&world_manifold);

	Collision collision;
	collision.other = actorB;
	collision.point = INVALID_VEC;
	collision.normal = INVALID_VEC;
	collision.relative_velocity = fixtureA->GetBody()->GetLinearVelocity() - fixtureB->GetBody()->GetLinearVelocity();

	if (!fixtureA->IsSensor() && !fixtureB->IsSensor())
	{
		actorA->OnCollisionExit(collision);
		collision.other = actorA;
		actorB->OnCollisionExit(collision);
	}

	if (fixtureA->IsSensor() && fixtureB->IsSensor())
	{
		actorA->OnTriggerExit(collision);
		collision.other = actorA;
		actorB->OnTriggerExit(collision);
	}
}
