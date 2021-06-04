#pragma once
#include "Actor.h"
#include "LevelData.h"
#include "Scene.h"

class Goal :
    public Actor
{
public:
    Goal(const LevelData& data, Scene* scene) : Actor("Goal"), scene(scene), tex("EntityGoal") {
        pos = data.goalPos;
        hitbox = sf::FloatRect(0, 0, 1.0f, 1.0f);
        spr.setTexture(tex.getTexture());
        spr.setPosition(toPixels(pos.x), toPixels(pos.y));
    }

    void update() override {

    }

    void draw(ic::Window& window) const override {
        window.window->draw(spr);
    }

    virtual void setAirborne(const bool airborne) {}


    void onCollision(std::shared_ptr<Actor> actor) {
        if (actor->getName() == "Player") {
            scene->setChangeLevel();
        }
    }

    void onTileCollision(const Tile& tile, const CollisionType& type) override {}

    bool collidable() override { return false; }

private:
    Scene* scene;
    sf::Sprite spr;
    ic::Texture tex;
};

