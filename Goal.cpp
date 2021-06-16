#include "stdafx.h"
#include "Goal.h"
#include "LevelData.h"
#include "Scene.h"
#include "Window.h"
#include "ActorData.h"


Goal::Goal(const ActorData& data) : Actor("Goal"), scene(data.scene), tex("EntityGoal") {
    pos = sf::Vector2f(toMeters(data.findKey<float>("x")), toMeters(data.findKey<float>("y")) - 1.0f);
    hitbox = sf::FloatRect(0, 0, 1.0f, 1.0f);
    spr.setTexture(tex.getTexture());
    spr.setPosition(toPixels(pos.x), toPixels(pos.y));
}

void Goal::update() {

}

void Goal::draw(Window& window) const {
    window.window->draw(spr);
}

void Goal::setAirborne(const bool airborne) {}


void Goal::onCollision(std::shared_ptr<Actor> actor) {
    if (actor->getName() == "Player") {
        scene->setChangeLevel();
    }
}

void Goal::onTileCollision(const Tile& tile, const CollisionType& type) {}

bool Goal::collidable() { return false; }