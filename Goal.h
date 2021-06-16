#pragma once
#include "Actor.h"
#include "Texture.h"

class Window;

class ActorData;
class Scene;
class Tile;
enum class CollisionType;

class Goal :
    public Actor
{
public:
    Goal(const ActorData& data);

    void update() override;

    void draw(Window& window) const override;

    virtual void setAirborne(const bool airborne);


    void onCollision(std::shared_ptr<Actor> actor);

    void onTileCollision(const Tile& tile, const CollisionType& type) override;

    bool collidable() override;

private:
    Scene* scene;
    sf::Sprite spr;
    Texture tex;
};

