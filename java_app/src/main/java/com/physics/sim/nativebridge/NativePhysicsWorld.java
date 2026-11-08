package com.physics.sim.nativebridge;

/**
 * NativePhysicsWorld — thin JNI wrapper around the C++ physics engine.
 *
 * Scene index:
 *   0 = Falling Balls
 *   1 = Projectile Motion
 *   2 = Bouncing Balls
 *   3 = Simple Pendulum
 *   4 = Double Pendulum
 */
public class NativePhysicsWorld {

    static {
        System.loadLibrary("physengine");
    }

    // ── Scene lifecycle ──────────────────────────────────────────
    public native void createScene(int sceneType);
    public native void step(int steps);

    // ── Body queries ─────────────────────────────────────────────
    public native int getBodyCount();
    public native double[] getBodyPositions();
    public native double[] getBodyVelocities();
    public native double[] getBodyState(int index);

    // ── Pendulum ─────────────────────────────────────────────────
    public native double[] getPendulumState();
    public native double[] getDoublePendulumState();

    // ── World ────────────────────────────────────────────────────
    public native double[] getWorldBounds();
    public native void addRandomBall();

    // ── Physics parameters ───────────────────────────────────────
    public native void setGravity(double gravity);
    public native void setAirResistance(double k);
    public native void setElasticity(double e);
    public native void setFriction(double f);

    // ── Stats ────────────────────────────────────────────────────
    public native double getKineticEnergy();
    public native double getPotentialEnergy();
    public native double getTotalEnergy();
    public native int getCollisionCount();
}