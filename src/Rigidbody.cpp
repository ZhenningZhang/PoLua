#include "Rigidbody.h"

#include "Actor.h"
#include "Helper.h"

void Rigidbody::OnStart()
{
	CreateRigidbody();
}

void Rigidbody::OverrideVariables(const rapidjson::Value& component_json)
{
	// Get values from json and create Rigidbody
	for (rapidjson::Value::ConstMemberIterator itr = component_json.MemberBegin(); itr != component_json.MemberEnd(); ++itr)
	{
		const std::string& variable_name = itr->name.GetString();

		if (variable_name == "type")
			continue;

		//std::cout << "Overriding " << variable_name << std::endl;

		const rapidjson::Value& value = itr->value;

		if (variable_name == "x")
			x = value.GetFloat();
		else if (variable_name == "y")
			y = value.GetFloat();
		else if (variable_name == "body_type")
			body_type = value.GetString();
		else if (variable_name == "precise")
			precise = value.GetBool();
		else if (variable_name == "gravity_scale")
			gravity_scale = value.GetFloat();
		else if (variable_name == "density")
			density = value.GetFloat();
		else if (variable_name == "angular_friction")
			angular_friction = value.GetFloat();
		else if (variable_name == "rotation")
			rotation = value.GetFloat();
		else if (variable_name == "has_collider")
			has_collider = value.GetBool();
		else if (variable_name == "has_trigger")
			has_trigger = value.GetBool();
		else if (variable_name == "collider_type")
			collider_type = value.GetString();
		else if (variable_name == "width")
			width = value.GetFloat();
		else if (variable_name == "height")
			height = value.GetFloat();
		else if (variable_name == "radius")
			radius = value.GetFloat();
		else if (variable_name == "friction")
			friction = value.GetFloat();
		else if (variable_name == "bounciness")
			bounciness = value.GetFloat();
		else if (variable_name == "trigger_type")
			trigger_type = value.GetString();
		else if (variable_name == "trigger_width")
			trigger_width = value.GetFloat();
		else if (variable_name == "trigger_height")
			trigger_height = value.GetFloat();
		else if (variable_name == "trigger_radius")
			trigger_radius = value.GetFloat();
	}
}

void Rigidbody::CreateRigidbody()
{
	if (world == nullptr)
	{
		world = new b2World(b2Vec2(0.0f, 9.8f));
		world->SetContactListener(&contact_listener);
	}

	// Create body from b2BodyDef
	b2BodyDef body_def;

	body_def.position = b2Vec2(x, y);

	if (body_type == "dynamic")
		body_def.type = b2_dynamicBody;
	else if (body_type == "kinematic")
		body_def.type = b2_kinematicBody;
	else if (body_type == "static")
		body_def.type = b2_staticBody;

	body_def.bullet = precise;
	body_def.gravityScale = gravity_scale;
	body_def.angularDamping = angular_friction;
	body_def.angle = rotation * (b2_pi / 180.0f);

	body = world->CreateBody(&body_def);

	if (!has_collider && !has_trigger)
	{
		// Disclaimer: This is directly copy and paste
		b2PolygonShape phantom_shape;
		phantom_shape.SetAsBox(width * 0.5f, height * 0.5f);

		b2FixtureDef phantom_fixture_def;
		phantom_fixture_def.shape = &phantom_shape;
		phantom_fixture_def.density = density;

		// Because it is a sensor (with no callback even), no collisions will ever occur
		phantom_fixture_def.isSensor = true;
		body->CreateFixture(&phantom_fixture_def);
	}

	if (has_collider)
	{
		b2FixtureDef fixture_def;
		fixture_def.density = density;
		fixture_def.isSensor = false;

		// misc
		b2PolygonShape box_shape;
		b2CircleShape circle_shape;

		if (collider_type == "box")
		{
			box_shape.SetAsBox(width * 0.5f, height * 0.5f);
			fixture_def.shape = &box_shape;
		}
		else
		{
			circle_shape.m_radius = radius;
			fixture_def.shape = &circle_shape;
		}

		fixture_def.friction = friction;
		fixture_def.restitution = bounciness;

		// HW8 Test Suite #4: OnCollision (This implementation is kind of weird)
		fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(actor);

		// HW8 Test Suite #9 Hint
		fixture_def.filter.categoryBits = 0x0001;
		fixture_def.filter.maskBits = 0x0001;

		body->CreateFixture(&fixture_def);
	}

	if (has_trigger)
	{
		b2FixtureDef fixture_def;
		fixture_def.density = density;
		fixture_def.isSensor = true;

		b2PolygonShape box_shape;
		b2CircleShape circle_shape;

		if (trigger_type == "box")
		{
			box_shape.SetAsBox(trigger_width * 0.5f, trigger_height * 0.5f);
			fixture_def.shape = &box_shape;
		}
		else
		{
			circle_shape.m_radius = trigger_radius;
			fixture_def.shape = &circle_shape;
		}

		// HW8 Test Suite #5: OnTrigger (This implementation is kind of weird)
		fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(actor);

		// HW8 Test Suite #9 Hint
		fixture_def.filter.categoryBits = 0x0002;
		fixture_def.filter.maskBits = 0x0002;

		body->CreateFixture(&fixture_def);
	}
}

b2Vec2 Rigidbody::GetPosition()
{
	if (body == nullptr)
		return b2Vec2(x, y);
	return body->GetPosition();
}

float Rigidbody::GetRotation()
{
	if (body == nullptr)
		return rotation;
	return body->GetAngle() * (180.0f / b2_pi);
}

float Rigidbody::GetWidth() {
	return width;
}

float Rigidbody::GetHeight() {
	return height;
}

b2Vec2 Rigidbody::GetVelocity()
{
	return body->GetLinearVelocity();
}

float Rigidbody::GetAngularVelocity()
{
	return body->GetAngularVelocity() * (180.0f / b2_pi);
}

float Rigidbody::GetGravityScale()
{
	return body->GetGravityScale();
}

b2Vec2 Rigidbody::GetUpDirection()
{
	float angle = body->GetAngle();

	b2Vec2 result = b2Vec2(glm::sin(angle), -glm::cos(angle));
	result.Normalize();

	return result;
}

b2Vec2 Rigidbody::GetRightDirection()
{
	float angle = body->GetAngle();

	b2Vec2 result = b2Vec2(glm::cos(angle), glm::sin(angle));
	result.Normalize();

	return result;
}

void Rigidbody::AddForce(const b2Vec2& force)
{
	body->ApplyForceToCenter(force, true);
}

void Rigidbody::SetVelocity(const b2Vec2& velocity)
{
	body->SetLinearVelocity(velocity);
}

void Rigidbody::SetPosition(const b2Vec2& position)
{
	if (body == nullptr)
		std::tie(x, y) = std::make_pair(position.x, position.y);
	else
		body->SetTransform(position, body->GetAngle());
}

void Rigidbody::SetRotation(const float degrees_clockwise)
{
	body->SetTransform(body->GetPosition(), degrees_clockwise * (b2_pi / 180.0f));
}

void Rigidbody::SetAngularVelocity(const float degrees_clockwise)
{
	body->SetAngularVelocity(degrees_clockwise * (b2_pi / 180.0f));
}

void Rigidbody::SetGravityScale(const float gravity_scale)
{
	body->SetGravityScale(gravity_scale);
}

void Rigidbody::SetUpDirection(b2Vec2 direction)
{
	direction.Normalize();
	float new_angle_radians = glm::atan(direction.x, -direction.y);
	body->SetTransform(body->GetPosition(), new_angle_radians);
}

void Rigidbody::SetRightDirection(b2Vec2 direction)
{
	direction.Normalize();
	float new_angle_radians = glm::atan(direction.x, -direction.y) - b2_pi / 2.0f;
	body->SetTransform(body->GetPosition(), new_angle_radians);
}

void Rigidbody::OnDestroy()
{
	// One of ways to check if this is a template
	if (body == nullptr)
		return;

	//std::cout << Helper::GetFrameNumber() << " Rigidbody Destroyed - " << actor->actor_name << std::endl;
	
	world->DestroyBody(body);
}