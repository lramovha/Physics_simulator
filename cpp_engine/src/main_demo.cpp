#include "physengine/scenes/FallingBallsScene.hpp"
#include "physengine/scenes/ProjectileScene.hpp"
#include "physengine/scenes/PendulumScene.hpp"
#include "physengine/scenes/BouncingBallsScene.hpp"
#include "physengine/scenes/DoublePendulumScene.hpp"
#include <iostream>

int main() {
    std::cout << "=== Physics Simulator - Collision Test Demo ===\n\n";

    // Choose which scene to run:
    // FallingBallsScene scene;
    // ProjectileScene scene;
    // PendulumScene scene;
    BouncingBallsScene scene;        // ← Best for testing collisions
    // DoublePendulumScene scene;

    scene.init();
    std::cout << "Running: " << scene.getName() << "\n";
    std::cout << "Bodies: " << scene.world.bodies.size() << "\n\n";

    std::cout << "Simulating 800 steps...\n";
    std::cout << "Watch for ball-to-ball collisions!\n\n";

    for (int i = 0; i < 800; i++) {
        scene.update();

        // Print status every 80 steps
        if (i % 80 == 0 || i == 799) {
            std::cout << "[Step " << i << "] ";
            scene.world.printStatus();
        }
    }

    std::cout << "\n=== Demo Finished ===\n";
    std::cout << "Tip: Try changing to FallingBallsScene or BouncingBallsScene to see collisions.\n";
    
    return 0;
}