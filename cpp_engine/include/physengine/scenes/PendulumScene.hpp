#ifndef PENDULUM_SCENE_HPP
#define PENDULUM_SCENE_HPP

#include "BaseScene.hpp"

class PendulumScene : public BaseScene {
public:
    void init() override;
    const char* getName() const override { return "Simple Pendulum"; }
};

#endif