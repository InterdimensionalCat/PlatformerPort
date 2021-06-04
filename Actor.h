#pragma once
#include "Window.h"
#include "Component.h"


class Tile;

enum class CollisionType {
	Wall,
	Floor,
	Ceiling
};

class Actor
{
public:
	Actor(const std::string& name) : name(name) {}
	virtual void update() = 0;
	virtual void draw(ic::Window& window) const = 0;
	virtual void onCollision(std::shared_ptr<Actor> actor) = 0;
	virtual void onTileCollision(const Tile& tile, const CollisionType& type) = 0;
	virtual void setAirborne(const bool airborne) = 0;
	virtual bool isAirborne() { return false; }
	virtual bool collidable() { return true; }
	virtual void despawn() {}
	std::string name;

	const std::string& getName() const { return name; }


	virtual sf::FloatRect getPosAdjustedAABB() {
		return sf::FloatRect(hitbox.left + pos.x, hitbox.top + pos.y, hitbox.width, hitbox.height);
	}


	void setPosX(const float newx) {
		pos.x = newx;
	}

	void setPosY(const float newy) {
		pos.y = newy;
	}

	void setVelX(const float newVelX) {
		vel.x = newVelX;
	}

	void setVelY(const float newVelY) {
		vel.y = newVelY;
	}

	void moveX(const float motionX) {
		vel.x += motionX;
	}

	void moveY(const float motionY) {
		vel.y += motionY;
	}

	void move(const float motionX, const float motionY) {
		moveX(motionX);
		moveY(motionY);
	}

	float getPosX() {
		return pos.x;
	}

	float getPosY() {
		return pos.y;
	}

	float getVelX() {
		return vel.x;
	}

	float getVelY() {
		return vel.y;
	}


	float collisionTolerenceY = 0.0f;

	sf::FloatRect hitbox = sf::FloatRect(0, 0, 0, 0);
	sf::Vector2f pos = sf::Vector2f(0, 0);
	sf::Vector2f vel = sf::Vector2f(0, 0);

	Scene* scene;

//private:
//
//	friend class Player;
//	friend class PlayerLogic;
//	friend class PlayerGraphics;
//

};

