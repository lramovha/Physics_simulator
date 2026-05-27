#ifndef BASE_SCENE_HPP
#define BASE_SCENE_HPP

#include "physengine/PhysicsWorld.hpp"

class BaseScene {
public:
    PhysicsWorld world;

    virtual ~BaseScene() = default;
    
    virtual void init() = 0;
    virtual void update() { world.step(); }
    virtual void reset();

    virtual const char* getName() const = 0;
};

#endif