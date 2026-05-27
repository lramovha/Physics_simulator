#ifndef FALLING_BALLS_SCENE_HPP
#define FALLING_BALLS_SCENE_HPP

#include "BaseScene.hpp"

class FallingBallsScene : public BaseScene {
public:
    void init() override;
    const char* getName() const override { return "Falling Balls"; }
};

#endif