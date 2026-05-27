#include "physengine/scenes/FallingBallsScene.hpp"

void FallingBallsScene::init() {
    // Ground
    auto ground = std::make_unique<RigidBody>(1.0, Vector3(0, -0.5, 0));
    ground->isStatic = true;
    world.addBody(std::move(ground));

    // Several falling balls
    for (int i = 0; i < 8; i++) {
        double x = (i - 4) * 1.5;
        auto ball = std::make_unique<RigidBody>(1.0, Vector3(x, 5.0 + i*2, 0));
        ball->isStatic = false;
        world.addBody(std::move(ball));
    }
}