#ifndef PROJECTILE_SCENE_HPP
#define PROJECTILE_SCENE_HPP

#include "BaseScene.hpp"

class ProjectileScene : public BaseScene {
public:
    void init() override;
    const char* getName() const override { return "Projectile Motion"; }
};

#endif