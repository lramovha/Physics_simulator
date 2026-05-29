#include "physengine/scenes/PendulumScene.hpp"

void PendulumScene::init() {
    // Fixed pivot (static body)
    auto pivot = std::make_unique<RigidBody>(1.0, Vector3(0, 8, 0));
    pivot->isStatic = true;
    world.addBody(std::move(pivot));

    // Pendulum bob
    auto bob = std::make_unique<RigidBody>(1.0, Vector3(4, 4, 0));
    bob->isStatic = false;
    bob->velocity = Vector3(0, 0, 0);
    world.addBody(std::move(bob));
}