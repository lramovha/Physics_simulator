#include <catch2/catch_test_macros.hpp>
#include "physengine/PhysicsWorld.hpp"

TEST_CASE("PhysicsWorld initializes") {
    PhysicsWorld world;

    REQUIRE_NOTHROW(world.hello());
}