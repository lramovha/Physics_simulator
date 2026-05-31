#include "physengine/scenes/FallingBallsScene.hpp"

void FallingBallsScene::init() {
    world.clear();

    /* ── World boundary ─────────────────────────────────────────
     * Balls fall in a box: x [-15, 15], y [-8, 25]
     * Z kept narrow since this is a 2-D scene
     * ─────────────────────────────────────────────────────────── */
    world.bounds.minX    = -15.0;
    world.bounds.maxX    =  15.0;
    world.bounds.minY    =  -8.0;
    world.bounds.maxY    =  25.0;
    world.bounds.minZ    =  -1.0;
    world.bounds.maxZ    =   1.0;
    world.bounds.enabled =  true;

    /* ── Ground (static, sits at the floor) ─────────────────────
     * Still here as a visual reference body for the renderer
     * ─────────────────────────────────────────────────────────── */
    auto ground = std::make_unique<RigidBody>(1.0, Vector3(0, -0.5, 0));
    ground->isStatic = true;
    world.addBody(std::move(ground));

    /* ── Falling balls ──────────────────────────────────────────
     * Spread across X, staggered heights, different restitution
     * values so they bounce to different heights — more interesting
     * ─────────────────────────────────────────────────────────── */
    double startX[8]  = {-10.5, -7.5, -4.5, -1.5, 1.5, 4.5, 7.5, 10.5};
    double restit[8]  = { 0.90,  0.75,  0.85,  0.60,
                           0.80,  0.70,  0.95,  0.65 };

    for (int i = 0; i < 8; i++) {
        auto ball = std::make_unique<RigidBody>(1.0,
                        Vector3(startX[i], 5.0 + i * 2.0, 0));
        ball->isStatic    = false;
        ball->restitution = restit[i];
        ball->radius      = 0.5;
        world.addBody(std::move(ball));
    }
}


// #include "physengine/scenes/FallingBallsScene.hpp"

// void FallingBallsScene::init() {
//     // Ground
//     auto ground = std::make_unique<RigidBody>(1.0, Vector3(0, -0.5, 0));
//     ground->isStatic = true;
//     world.addBody(std::move(ground));

//     // Several falling balls
//     for (int i = 0; i < 8; i++) {
//         double x = (i - 4) * 1.5;
//         auto ball = std::make_unique<RigidBody>(1.0, Vector3(x, 5.0 + i*2, 0));
//         ball->isStatic = false;
//         world.addBody(std::move(ball));
//     }
// }