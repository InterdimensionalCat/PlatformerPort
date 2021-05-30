#include "stdafx.h"
#include "PlayerGraphics.h"
#include "Player.h"


void PlayerGraphics::draw(ic::Window& window) const {
    sf::RectangleShape rect;
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(5);
    rect.setFillColor(sf::Color(0, 0, 0, 0));
    rect.setPosition(sf::Vector2f(toPixels(player->logic->pos.x), toPixels(player->logic->pos.y)));
    rect.setSize(sf::Vector2f(toPixels(1.0f), toPixels(1.0f)));
    window.window->draw(rect);
}