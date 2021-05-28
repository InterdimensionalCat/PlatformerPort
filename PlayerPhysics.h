#pragma once
#include "Component.h"
#include "LevelData.h"
#inclue "Player.h"

class PlayerPhysics :
    public PhysicsComponent
{
    PlayerPhysics(const LevelData& data) : PhysicsComponent(data) {}



private:

    //all in meters
    sf::FloatRect hitbox = sf::FloatRect(0,0, toMeters(64-25), toMeters(64-10));
    sf::Vector2f pos = sf::Vector2f(0,0);
    sf::Vector2f vel = sf::Vector2f(0,0);
};

