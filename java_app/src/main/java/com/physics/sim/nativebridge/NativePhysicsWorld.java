package com.physics.sim.nativebridge;

/**
 * NativePhysicsWorld
 *
 * Thin Java wrapper around the C++ physics engine (libphysengine).
 * All heavy computation stays on the C++ side — this class only
 * declares the native method signatures and loads the shared library.
 *
 * Scene index mapping (matches PhysicsJNI.cpp switch):
 *   0 = Falling Balls
 *   1 = Projectile Motion
 *   2 = Bouncing Balls
 *   3 = Simple Pendulum
 */
public class NativePhysicsWorld {

    static {
        System.loadLibrary("physengine");
    }

    // ── Scene lifecycle ──────────────────────────────────────────

    /** Create and initialise a scene. Destroys the previous scene. */
    public native void createScene(int sceneType);

    /** Advance the simulation by the given number of physics steps. */
    public native void step(int steps);

    // ── Generic body state (ball scenes) ────────────────────────

    /** Number of RigidBody objects in the current scene's PhysicsWorld. */
    public native int getBodyCount();

    /**
     * Flat array of body positions: [x0,y0,z0, x1,y1,z1, ...]
     * Length = getBodyCount() * 3
     */
    public native double[] getBodyPositions();

    // ── Pendulum-specific state (scene 3 only) ───────────────────

    /**
     * Returns 6 doubles describing the pendulum:
     *   [0] pivotX   — world-space X of the fixed pivot
     *   [1] pivotY   — world-space Y of the fixed pivot
     *   [2] bobX     — world-space X of the bob
     *   [3] bobY     — world-space Y of the bob
     *   [4] theta    — current angle from vertical (radians)
     *   [5] omega    — current angular velocity (rad/s)
     *
     * Returns all zeroes if the active scene is not a pendulum.
     */
    public native double[] getPendulumState();

    /**
    * Returns 10 doubles for Double Pendulum:
    * [pivotX, pivotY, bob1X, bob1Y, bob2X, bob2Y, theta1, omega1, theta2, omega2]
    */
    public native double[] getDoublePendulumState();

    // ── World boundary ───────────────────────────────────────────

    /**
     * Returns 6 doubles: [minX, maxX, minY, maxY, minZ, maxZ]
     * Used by the renderer to draw the boundary rectangle.
     * Returns all zeroes if no scene is active or bounds are disabled.
     */
    public native double[] getWorldBounds();

    // NEW: Add a random dynamic ball
    public native void addRandomBall();

    public native void setGravity(double gravity);
}
