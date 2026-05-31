#include "physengine/scenes/ProjectileScene.hpp"

void ProjectileScene::init() {
    world.clear();

    /* ── World boundary ─────────────────────────────────────────
     * Wide box so the projectile arc fits fully before bouncing
     * ─────────────────────────────────────────────────────────── */
    world.bounds.minX    = -22.0;
    world.bounds.maxX    =  22.0;
    world.bounds.minY    =  -8.0;
    world.bounds.maxY    =  20.0;
    world.bounds.minZ    =  -1.0;
    world.bounds.maxZ    =   1.0;
    world.bounds.enabled =  true;

    /* ── Ground (static reference body) ─────────────────────── */
    auto ground = std::make_unique<RigidBody>(1.0, Vector3(0, 0, 0));
    ground->isStatic = true;
    world.addBody(std::move(ground));

    /* ── Projectile ──────────────────────────────────────────────
     * Launched from bottom-left at 45 degrees (vx == vy)
     * restitution 0.55 — it bounces but loses energy each time
     * ─────────────────────────────────────────────────────────── */
    auto projectile = std::make_unique<RigidBody>(1.0,
                          Vector3(-18.0, 2.0, 0));
    projectile->isStatic    = false;
    projectile->velocity    = Vector3(15.0, 15.0, 0);
    projectile->restitution = 0.55;
    projectile->radius      = 0.5;

    world.addBody(std::move(projectile));
}


// #include "physengine/scenes/ProjectileScene.hpp"

// void ProjectileScene::init() {
//     // Ground
//     auto ground = std::make_unique<RigidBody>(1.0, Vector3(0, 0, 0));
//     ground->isStatic = true;
//     world.addBody(std::move(ground));

//     // Projectile launched at angle
//     auto projectile = std::make_unique<RigidBody>(1.0, Vector3(-10, 2, 0));
//     projectile->isStatic = false;
    
//     // Initial velocity (like throwing)
//     projectile->velocity = Vector3(15, 15, 0);   // vx=15, vy=15
    
//     world.addBody(std::move(projectile));
// }