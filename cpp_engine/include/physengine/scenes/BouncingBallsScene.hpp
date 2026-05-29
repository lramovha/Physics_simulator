#ifndef BOUNCING_BALLS_SCENE_HPP
#define BOUNCING_BALLS_SCENE_HPP

#include "BaseScene.hpp"

class BouncingBallsScene : public BaseScene {
public:
    void init() override;
    const char* getName() const override { return "Bouncing Balls"; }
};

#endif