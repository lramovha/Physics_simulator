#include <jni.h>
#include "physengine/PhysicsWorld.hpp"
#include "physengine/scenes/BaseScene.hpp"
#include "physengine/scenes/FallingBallsScene.hpp"
#include "physengine/scenes/ProjectileScene.hpp"
#include "physengine/scenes/BouncingBallsScene.hpp"
#include "physengine/scenes/PendulumScene.hpp"
#include "physengine/scenes/DoublePendulumScene.hpp"

/* ─────────────────────────────────────────────────────────────────
 * Global scene pointer
 * Simple single-scene model: only one scene runs at a time.
 * The Java side calls createScene() to switch.
 * ───────────────────────────────────────────────────────────────── */
static BaseScene* currentScene = nullptr;

extern "C" {

/* ── createScene ────────────────────────────────────────────────
 * sceneType:
 *   0 = Falling Balls
 *   1 = Projectile Motion
 *   2 = Bouncing Balls
 *   3 = Simple Pendulum
 * ─────────────────────────────────────────────────────────────── */
JNIEXPORT void JNICALL
Java_com_physics_sim_nativebridge_NativePhysicsWorld_createScene
(JNIEnv* /*env*/, jobject /*obj*/, jint sceneType)
{
    delete currentScene;
    currentScene = nullptr;

    switch (sceneType) {
        case 0:  currentScene = new FallingBallsScene();  break;
        case 1:  currentScene = new ProjectileScene();    break;
        case 2:  currentScene = new BouncingBallsScene(); break;
        case 3:  currentScene = new PendulumScene();      break;
        case 4: currentScene = new DoublePendulumScene(); break;    // DoublePendelum Scene
        default: currentScene = new FallingBallsScene();  break;
    }
    currentScene->init();
}

/* ── step ────────────────────────────────────────────────────────
 * Advance the simulation by `steps` ticks.
 * PendulumScene overrides update() so it calls rk4_step directly;
 * all other scenes call world.step() which includes boundary clamping.
 * ─────────────────────────────────────────────────────────────── */
JNIEXPORT void JNICALL
Java_com_physics_sim_nativebridge_NativePhysicsWorld_step
(JNIEnv* /*env*/, jobject /*obj*/, jint steps)
{
    if (!currentScene) return;
    for (int i = 0; i < steps; i++) {
        currentScene->update();
    }
}

/* ── getBodyCount ─────────────────────────────────────────────── */
JNIEXPORT jint JNICALL
Java_com_physics_sim_nativebridge_NativePhysicsWorld_getBodyCount
(JNIEnv* /*env*/, jobject /*obj*/)
{
    if (!currentScene) return 0;
    return static_cast<jint>(currentScene->world.bodies.size());
}

/* ── getBodyPositions ────────────────────────────────────────────
 * Returns a flat double array: [x0, y0, z0,  x1, y1, z1, ...]
 * Used by the generic ball renderer in MainApp.java
 * ─────────────────────────────────────────────────────────────── */
JNIEXPORT jdoubleArray JNICALL
Java_com_physics_sim_nativebridge_NativePhysicsWorld_getBodyPositions
(JNIEnv* env, jobject /*obj*/)
{
    if (!currentScene) return nullptr;

    int count = static_cast<int>(currentScene->world.bodies.size());
    jdoubleArray result = env->NewDoubleArray(count * 3);
    if (!result) return nullptr;

    jdouble* data = env->GetDoubleArrayElements(result, nullptr);
    for (int i = 0; i < count; i++) {
        const auto& body = currentScene->world.bodies[i];
        data[i * 3]     = body->position.x();
        data[i * 3 + 1] = body->position.y();
        data[i * 3 + 2] = body->position.z();
    }
    env->ReleaseDoubleArrayElements(result, data, 0);
    return result;
}

/* ── getPendulumState ────────────────────────────────────────────
 * Only valid when scene 3 (PendulumScene) is active.
 * Returns 6 doubles: [pivotX, pivotY, bobX, bobY, theta, omega]
 *
 * Java renderer uses:
 *   pivotX/Y → fixed pin point
 *   bobX/Y   → where to draw the bob circle
 *   theta    → angle label or trail overlay
 *   omega    → optional speed indicator
 * ─────────────────────────────────────────────────────────────── */
JNIEXPORT jdoubleArray JNICALL
Java_com_physics_sim_nativebridge_NativePhysicsWorld_getPendulumState
(JNIEnv* env, jobject /*obj*/)
{
    jdoubleArray result = env->NewDoubleArray(6);
    if (!result) return nullptr;

    PendulumScene* p = dynamic_cast<PendulumScene*>(currentScene);
    if (!p) {
        /* Not a pendulum scene — return all zeroes */
        jdouble zeros[6] = {0, 0, 0, 0, 0, 0};
        env->SetDoubleArrayRegion(result, 0, 6, zeros);
        return result;
    }

    jdouble data[6] = {
        p->pivotX,    p->pivotY,
        p->getBobX(), p->getBobY(),
        p->theta,     p->omega
    };
    env->SetDoubleArrayRegion(result, 0, 6, data);
    return result;
}

// Native method for the DoublePandulumState
JNIEXPORT jdoubleArray JNICALL
Java_com_physics_sim_nativebridge_NativePhysicsWorld_getDoublePendulumState
(JNIEnv* env, jobject /*obj*/)
{
    jdoubleArray result = env->NewDoubleArray(10);  // pivot + bob1 + bob2 + 4 states
    if (!result) return nullptr;

    DoublePendulumScene* dp = dynamic_cast<DoublePendulumScene*>(currentScene);
    if (!dp) {
        jdouble zeros[10] = {0};
        env->SetDoubleArrayRegion(result, 0, 10, zeros);
        return result;
    }

    jdouble data[10] = {
        dp->pivotX, dp->pivotY,
        dp->getBob1X(), dp->getBob1Y(),
        dp->getBob2X(), dp->getBob2Y(),
        dp->theta1, dp->omega1,
        dp->theta2, dp->omega2
    };
    env->SetDoubleArrayRegion(result, 0, 10, data);
    return result;
}


/* ── getWorldBounds ──────────────────────────────────────────────
 * Returns 6 doubles: [minX, maxX, minY, maxY, minZ, maxZ]
 * Java renderer uses this to draw the boundary box on screen.
 * Returns all zeroes if bounds are disabled or no scene is active.
 * ─────────────────────────────────────────────────────────────── */
JNIEXPORT jdoubleArray JNICALL
Java_com_physics_sim_nativebridge_NativePhysicsWorld_getWorldBounds
(JNIEnv* env, jobject /*obj*/)
{
    jdoubleArray result = env->NewDoubleArray(6);
    if (!result) return nullptr;

    if (!currentScene || !currentScene->world.bounds.enabled) {
        jdouble zeros[6] = {0, 0, 0, 0, 0, 0};
        env->SetDoubleArrayRegion(result, 0, 6, zeros);
        return result;
    }

    const WorldBounds& b = currentScene->world.bounds;
    jdouble data[6] = { b.minX, b.maxX, b.minY, b.maxY, b.minZ, b.maxZ };
    env->SetDoubleArrayRegion(result, 0, 6, data);
    return result;
}

// Add Random balls to the siulation
JNIEXPORT void JNICALL
Java_com_physics_sim_nativebridge_NativePhysicsWorld_addRandomBall
(JNIEnv* /*env*/, jobject /*obj*/)
{
    if (!currentScene) return;

    // Only add balls to scenes that use RigidBody system
    if (dynamic_cast<PendulumScene*>(currentScene) || 
        dynamic_cast<DoublePendulumScene*>(currentScene)) {
        return; // Don't add balls to pendulum scenes
    }

    // Generate random position and velocity
    double x = (rand() % 20) - 10.0;           // -10 to +10
    double y = 8.0 + (rand() % 12);            // 8 to 20
    double vx = (rand() % 11) - 5.0;           // -5 to +5
    double vy = (rand() % 8) - 2.0;            // -2 to +6

    auto newBall = std::make_unique<RigidBody>(1.0, Vector3(x, y, 0));
    newBall->isStatic = false;
    newBall->radius = 0.5;
    newBall->restitution = 0.75 + (rand() % 20) / 100.0; // 0.75 to 0.95
    newBall->velocity = Vector3(vx, vy, 0);

    currentScene->world.addBody(std::move(newBall));
}

} /* extern "C" */


// #include <jni.h>
// #include "physengine/PhysicsWorld.hpp"
// #include "physengine/scenes/BaseScene.hpp"
// #include "physengine/scenes/FallingBallsScene.hpp"
// #include "physengine/scenes/ProjectileScene.hpp"

// // Global scene pointer (simple approach for now)
// static BaseScene* currentScene = nullptr;

// extern "C" {

// JNIEXPORT void JNICALL Java_com_physics_sim_nativebridge_NativePhysicsWorld_createScene
// (JNIEnv* env, jobject obj, jint sceneType) {
//     if (currentScene) delete currentScene;

//     switch (sceneType) {
//         case 0: currentScene = new FallingBallsScene(); break;
//         case 1: currentScene = new ProjectileScene(); break;
//         default: currentScene = new FallingBallsScene(); break;
//     }
//     currentScene->init();
// }

// JNIEXPORT void JNICALL Java_com_physics_sim_nativebridge_NativePhysicsWorld_step
// (JNIEnv* env, jobject obj, jint steps) {
//     if (currentScene) {
//         for (int i = 0; i < steps; i++) {
//             currentScene->update();
//         }
//     }
// }

// JNIEXPORT jint JNICALL Java_com_physics_sim_nativebridge_NativePhysicsWorld_getBodyCount
// (JNIEnv* env, jobject obj) {
//     return currentScene ? (jint)currentScene->world.bodies.size() : 0;
// }

// JNIEXPORT jdoubleArray JNICALL Java_com_physics_sim_nativebridge_NativePhysicsWorld_getBodyPositions
// (JNIEnv* env, jobject obj) {
//     if (!currentScene) return nullptr;

//     int count = currentScene->world.bodies.size();
//     jdoubleArray result = env->NewDoubleArray(count * 3);
//     if (!result) return nullptr;

//     jdouble* data = env->GetDoubleArrayElements(result, nullptr);

//     for (int i = 0; i < count; i++) {
//         const auto& body = currentScene->world.bodies[i];
//         data[i*3]     = body->position.x();
//         data[i*3 + 1] = body->position.y();
//         data[i*3 + 2] = body->position.z();
//     }

//     env->ReleaseDoubleArrayElements(result, data, 0);
//     return result;
// }

// } // extern "C"