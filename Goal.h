#pragma once
#include "Actor.h"
#include "Texture.h"

namespace ic {
    class Window;
}

class LevelData;
class Scene;
class Tile;
enum class CollisionType;

class Goal :
    public Actor
{
public:
    Goal(const LevelData& data, Scene* scene);

    void update() override;

    void draw(ic::Window& window) const override;

    virtual void setAirborne(const bool airborne);


    void onCollision(std::shared_ptr<Actor> actor);

    void onTileCollision(const Tile& tile, const CollisionType& type) override;

    bool collidable() override;

private:
    Scene* scene;
    sf::Sprite spr;
    ic::Texture tex;
};

