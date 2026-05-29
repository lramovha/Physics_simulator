package com.physics.sim.nativebridge;

public class NativePhysicsWorld {

    static {
        System.loadLibrary("physengine");
    }

    public native void createScene(int sceneType);  // 0 = Falling Balls, 1 = Projectile
    public native void step(int steps);
    public native int getBodyCount();
    public native double[] getBodyPositions();
}