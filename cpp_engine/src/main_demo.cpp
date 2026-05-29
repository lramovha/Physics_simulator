#include "physengine/scenes/FallingBallsScene.hpp"
#include "physengine/scenes/ProjectileScene.hpp"
#include "physengine/scenes/PendulumScene.hpp"
#include "physengine/scenes/BouncingBallsScene.hpp"
#include <iostream>

int main() {
    std::cout << "=== Physics Simulator - Phase 6 (Scenes) ===\n\n";

    // Choose a scene to run:
    // FallingBallsScene scene;
    // ProjectileScene scene;
    PendulumScene scene;
    // BouncingBallsScene scene;

    scene.init();
    std::cout << "Running: " << scene.getName() << "\n\n";

    for (int i = 0; i < 500; i++) {
        scene.update();
        if (i % 60 == 0) {
            scene.world.printStatus();
        }
    }

    std::cout << "\n=== Demo Finished ===\n";
    return 0;
}