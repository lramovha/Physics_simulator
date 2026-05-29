package com.physics.sim;

import com.physics.sim.nativebridge.NativePhysicsWorld;

public class TestJNI {
    
    public static void main(String[] args) {
        System.out.println("=== Physics Simulator JNI Test ===\n");
        
        try {
            NativePhysicsWorld nativeWorld = new NativePhysicsWorld();
            
            System.out.println("1. Creating Falling Balls Scene...");
            nativeWorld.createScene(0);  // 0 = Falling Balls
            
            System.out.println("2. Stepping simulation 200 times...");
            nativeWorld.step(200);
            
            int bodyCount = nativeWorld.getBodyCount();
            System.out.println("3. Number of bodies: " + bodyCount);
            
            double[] positions = nativeWorld.getBodyPositions();
            System.out.println("4. Body positions received: " + positions.length + " values");
            
            // Print first 3 bodies
            for (int i = 0; i < Math.min(3, bodyCount); i++) {
                int idx = i * 3;
                System.out.printf("   Body %d: (%.2f, %.2f, %.2f)\n", 
                    i, positions[idx], positions[idx+1], positions[idx+2]);
            }
            
            System.out.println("\n✅ JNI Bridge Test PASSED!");
            
        } catch (UnsatisfiedLinkError e) {
            System.err.println("❌ Failed to load native library: " + e.getMessage());
            System.err.println("Make sure libphysengine.dll is in the correct location.");
        } catch (Exception e) {
            System.err.println("❌ Error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}