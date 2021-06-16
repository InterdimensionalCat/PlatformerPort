#include "stdafx.h"
#include "PlayerGraphics.h"
#include "Player.h"
#include "AnimationRegistry.h"
#include "LevelData.h"

PlayerGraphics::PlayerGraphics(const ActorData& data) : anim(getAnimFromName("PlayerIdle")) {}

void PlayerGraphics::changeState(const ActionState& state) {

    auto pos = anim.frame.getPosition();
    auto flipped = anim.flip;

    switch (state) {
    case ActionState::GroundStill:
        anim = getAnimFromName("PlayerIdle");
        break;
    case ActionState::GroundMove:
        anim = getAnimFromName("PlayerRun");
        break;
    case ActionState::GroundTurn:
        anim = getAnimFromName("PlayerTurn");
        break;
    case ActionState::Airborne:
        anim = getAnimFromName("PlayerJump");
        break;
    }

    anim.setPositition(pos);
    anim.setFlipped(flipped);
}

void PlayerGraphics::update() {

    auto hitbox = player->getPosAdjustedAABB();
    auto width = toMeters(anim.targetDim.x);
    auto height = toMeters(anim.targetDim.y);

    auto center = sf::Vector2f(hitbox.left + hitbox.width / 2.0f, hitbox.top + hitbox.height / 2.0f);

    anim.setPositition(sf::Vector2f(toPixels(center.x - width / 2.0f), toPixels(hitbox.top + hitbox.height - height)));

    if (player->getVelX() > 0) {
        anim.setFlipped(false);
    }

    if (player->getVelX() < 0) {
        anim.setFlipped(true);
    }
}


void PlayerGraphics::draw(Window& window) {

    anim.draw(window);

    sf::RectangleShape rect;
    switch (player->state) {
    case ActionState::Airborne:
        rect.setOutlineColor(sf::Color::Black);
        break;
    case ActionState::GroundStill:
        rect.setOutlineColor(sf::Color::Red);
        break;
    case ActionState::GroundMove:
        rect.setOutlineColor(sf::Color::Blue);
        break;
    case ActionState::GroundTurn:
        rect.setOutlineColor(sf::Color::Yellow);
        break;
    }

    rect.setOutlineThickness(1);
    rect.setFillColor(sf::Color(0, 0, 0, 0));
    //rect.setPosition(sf::Vector2f(toPixels(player->pos.x), toPixels(player->pos.y)));

    auto hitbox = player->getPosAdjustedAABB();

    rect.setPosition(sf::Vector2f(toPixels(hitbox.left), toPixels(hitbox.top)));

    rect.setSize(sf::Vector2f(toPixels(hitbox.width), toPixels(hitbox.height)));
    //window.window->draw(rect);

    //sf::CircleShape shape;
    //shape.setRadius(1.0f);
    //shape.setFillColor(sf::Color::Yellow);
    //shape.setPosition(sf::Vector2f(toPixels(hitbox.left), toPixels(hitbox.top)));

    //window.window->draw(shape);
}