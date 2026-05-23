#include "physengine/PhysicsWorld.hpp"
#include "physmath/physmath.h"
#include <iostream>

PhysicsWorld::PhysicsWorld() {}

void PhysicsWorld::hello() const {
    std::cout << "PhysEngine C++ Layer v" << PHYSMATH_VERSION << " - Ready\n";
    physmath_hello();
}

std::string PhysicsWorld::getVersion() const {
    return PHYSMATH_VERSION;
}