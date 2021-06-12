#pragma once
#include "Animation.h"

class LevelData;
class Player;
enum class ActionState;
class Window;

class PlayerGraphics
{
public:
    PlayerGraphics(const LevelData& data);

    void changeState(const ActionState& state);

    void update();
    void draw(Window& window);
    Player* player;

    Animation anim;
};

