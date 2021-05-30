#pragma once
#include "Window.h"
#include "Component.h"

class Actor
{
public:
	Actor(const std::string& name) : name(name) {}
	virtual void update() = 0;
	virtual void draw(ic::Window& window) const = 0;
	std::string name;

	sf::FloatRect hitbox = sf::FloatRect(0, 0, 0, 0);
	sf::Vector2f pos = sf::Vector2f(0, 0);
	sf::Vector2f vel = sf::Vector2f(0, 0);
};

