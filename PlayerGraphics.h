#pragma once
#include "LevelData.h"

class Player;
namespace ic {
    class Window;
}

class PlayerGraphics
{
public:
    PlayerGraphics(const LevelData& data) {}
    void update() {}
    void draw(ic::Window& window) const;
    Player* player;
};

