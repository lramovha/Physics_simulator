#include "physengine/scenes/BaseScene.hpp"

void BaseScene::reset() {
    world.clear();
    init();
}