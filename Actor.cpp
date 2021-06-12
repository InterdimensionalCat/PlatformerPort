#include "stdafx.h"
#include "Actor.h"
#include "Window.h"
#include "Scene.h"


Actor::Actor(const std::string& name) : name(name) {}

bool Actor::isAirborne() { return false; }
bool Actor::collidable() { return true; }
void Actor::despawn() {}

const std::string& Actor::getName() const { return name; }

sf::FloatRect Actor::getPosAdjustedAABB() {
	return sf::FloatRect(hitbox.left + pos.x, hitbox.top + pos.y, hitbox.width, hitbox.height);
}


void Actor::setPosX(const float newx) {
	pos.x = newx;
}

void Actor::setPosY(const float newy) {
	pos.y = newy;
}

void Actor::setVelX(const float newVelX) {
	vel.x = newVelX;
}

void Actor::setVelY(const float newVelY) {
	vel.y = newVelY;
}

void Actor::moveX(const float motionX) {
	vel.x += motionX;
}

void Actor::moveY(const float motionY) {
	vel.y += motionY;
}

void Actor::move(const float motionX, const float motionY) {
	moveX(motionX);
	moveY(motionY);
}

float Actor::getPosX() {
	return pos.x;
}

float Actor::getPosY() {
	return pos.y;
}

float Actor::getVelX() {
	return vel.x;
}

float Actor::getVelY() {
	return vel.y;
}