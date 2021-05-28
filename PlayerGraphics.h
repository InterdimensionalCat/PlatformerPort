#pragma once
#include "Component.h"
#include "LevelData.h"
#include "Player.h"

class PlayerGraphics :
    public RenderComponent
{
public:
    PlayerGraphics(const LevelData& data) : RenderComponent(data) {}
};

