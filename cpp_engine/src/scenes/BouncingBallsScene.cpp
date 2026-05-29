#include "physengine/scenes/BouncingBallsScene.hpp"

void BouncingBallsScene::init() {
    // Floor
    auto floor = std::make_unique<RigidBody>(1.0, Vector3(0, 0, 0));
    floor->isStatic = true;
    world.addBody(std::move(floor));

    // Multiple bouncing balls
    for (int i = 0; i < 6; i++) {
        auto ball = std::make_unique<RigidBody>(1.0, Vector3(i*2 - 5, 3 + i*1.5, 0));
        ball->isStatic = false;
        ball->velocity = Vector3((rand() % 6) - 3, 5, 0);
        world.addBody(std::move(ball));
    }
}