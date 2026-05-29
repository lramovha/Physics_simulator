#include <jni.h>
#include "physengine/PhysicsWorld.hpp"
#include "physengine/scenes/BaseScene.hpp"
#include "physengine/scenes/FallingBallsScene.hpp"
#include "physengine/scenes/ProjectileScene.hpp"

// Global scene pointer (simple approach for now)
static BaseScene* currentScene = nullptr;

extern "C" {

JNIEXPORT void JNICALL Java_com_physics_sim_nativebridge_NativePhysicsWorld_createScene
(JNIEnv* env, jobject obj, jint sceneType) {
    if (currentScene) delete currentScene;

    switch (sceneType) {
        case 0: currentScene = new FallingBallsScene(); break;
        case 1: currentScene = new ProjectileScene(); break;
        default: currentScene = new FallingBallsScene(); break;
    }
    currentScene->init();
}

JNIEXPORT void JNICALL Java_com_physics_sim_nativebridge_NativePhysicsWorld_step
(JNIEnv* env, jobject obj, jint steps) {
    if (currentScene) {
        for (int i = 0; i < steps; i++) {
            currentScene->update();
        }
    }
}

JNIEXPORT jint JNICALL Java_com_physics_sim_nativebridge_NativePhysicsWorld_getBodyCount
(JNIEnv* env, jobject obj) {
    return currentScene ? (jint)currentScene->world.bodies.size() : 0;
}

JNIEXPORT jdoubleArray JNICALL Java_com_physics_sim_nativebridge_NativePhysicsWorld_getBodyPositions
(JNIEnv* env, jobject obj) {
    if (!currentScene) return nullptr;

    int count = currentScene->world.bodies.size();
    jdoubleArray result = env->NewDoubleArray(count * 3);
    if (!result) return nullptr;

    jdouble* data = env->GetDoubleArrayElements(result, nullptr);

    for (int i = 0; i < count; i++) {
        const auto& body = currentScene->world.bodies[i];
        data[i*3]     = body->position.x();
        data[i*3 + 1] = body->position.y();
        data[i*3 + 2] = body->position.z();
    }

    env->ReleaseDoubleArrayElements(result, data, 0);
    return result;
}

} // extern "C"