#pragma once
#include "Animation.h"

class Player;
enum class ActionState;
class Window;
class ActorData;

class PlayerGraphics
{
public:
    PlayerGraphics(const ActorData& data);

    void changeState(const ActionState& state);

    void update();
    void draw(Window& window);
    Player* player;

    Animation anim;
};

