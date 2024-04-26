#include "Physics.h"

#include "Rigidbody.h"

#include <algorithm>

luabridge::LuaRef Physics::Raycast(b2Vec2 pos, b2Vec2 dir, float dist)
{
    if (Rigidbody::world == nullptr || dist <= 0.0f)
        return luabridge::LuaRef(ComponentDB::lua_state);

    dir.Normalize();
    dir *= dist;

    RayCastCallback raycast_callback;
    Rigidbody::world->RayCast(&raycast_callback, pos, pos + dir);

    //std::cout << hit_results.size() << std::endl;

    if (hit_results.empty())
        return luabridge::LuaRef(ComponentDB::lua_state);

    std::sort(hit_results.begin(), hit_results.end());
    luabridge::LuaRef hit_result(ComponentDB::lua_state, hit_results.front());
    hit_results.clear();

    return hit_result;
}

luabridge::LuaRef Physics::RaycastAll(b2Vec2 pos, b2Vec2 dir, float dist)
{
    luabridge::LuaRef new_table = luabridge::newTable(ComponentDB::lua_state);

    if (Rigidbody::world == nullptr || dist <= 0.0f)
        return new_table;

    dir.Normalize();
    dir *= dist;

    RayCastCallback raycast_callback;
    Rigidbody::world->RayCast(&raycast_callback, pos, pos + dir);

    //std::cout << hit_results.size() << std::endl;

    if (hit_results.empty())
        return new_table;

    std::sort(hit_results.begin(), hit_results.end());

    // Lua is 1-indexing
    int counter = 1;

    for (const HitResult& hit_result : hit_results)
        new_table[counter++] = hit_result;
    
    hit_results.clear();

    return new_table;
}

float RayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
    HitResult hit_result;

    Actor* actor = reinterpret_cast<Actor*>(fixture->GetUserData().pointer);

    // Phantom Fixture
    if (actor == nullptr)
        return 1.0f;

    hit_result.actor = actor;
    hit_result.is_trigger = fixture->IsSensor();
    hit_result.normal = normal;
    hit_result.point = point;

    hit_result.fraction = fraction;

    Physics::hit_results.emplace_back(hit_result);

    return 1.0f;
}
