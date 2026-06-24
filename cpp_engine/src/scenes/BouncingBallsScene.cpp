#include "physengine/scenes/BouncingBallsScene.hpp"
#include <cstdlib>

void BouncingBallsScene::init() {
    world.clear();

    /* ── World boundary ─────────────────────────────────────────
     * Tighter box than falling balls — balls ricochet off all walls
     * ─────────────────────────────────────────────────────────── */
    world.bounds.minX    = -12.0;
    world.bounds.maxX    =  12.0;
    world.bounds.minY    =  -8.0;
    world.bounds.maxY    =  18.0;
    world.bounds.minZ    =  -1.0;
    world.bounds.maxZ    =   1.0;
    world.bounds.enabled =  true;

    /* ── Six bouncing balls ──────────────────────────────────────
     * Each gets a slightly different restitution (0.70 – 0.92)
     * and a random horizontal velocity so they don't all move
     * identically. srand is intentionally seeded with a fixed
     * value so the scene is repeatable — change the seed to vary.
     * ─────────────────────────────────────────────────────────── */
    srand(42);

    for (int i = 0; i < 6; i++) {
        double startX = (double)(i * 4) - 10.0;
        double startY = 3.0 + i * 1.5;
        double vx     = (double)((rand() % 7) - 3);   /* -3 to +3 */

        auto ball = std::make_unique<RigidBody>(1.0,
                        Vector3(startX, startY, 0));
        ball->isStatic    = false;
        ball->restitution = 0.70 + (double)(i % 4) * 0.074; /* 0.70, 0.77, 0.85, 0.92 cycling */
        ball->radius      = 0.5;
        ball->velocity    = Vector3(vx, 5.0, 0);

        world.addBody(std::move(ball));
    }
}
