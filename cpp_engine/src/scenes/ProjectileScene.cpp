#include "physengine/scenes/ProjectileScene.hpp"

void ProjectileScene::init() {
    // Ground
    auto ground = std::make_unique<RigidBody>(1.0, Vector3(0, 0, 0));
    ground->isStatic = true;
    world.addBody(std::move(ground));

    // Projectile launched at angle
    auto projectile = std::make_unique<RigidBody>(1.0, Vector3(-10, 2, 0));
    projectile->isStatic = false;
    
    // Initial velocity (like throwing)
    projectile->velocity = Vector3(15, 15, 0);   // vx=15, vy=15
    
    world.addBody(std::move(projectile));
}