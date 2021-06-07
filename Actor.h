#pragma once

namespace ic {
	class Window;
}

class Tile;
class Scene;

enum class CollisionType {
	Wall,
	Floor,
	Ceiling
};

class Actor
{
public:
	Actor(const std::string& name);
	virtual void update() = 0;
	virtual void draw(ic::Window& window) const = 0;
	virtual void onCollision(std::shared_ptr<Actor> actor) = 0;
	virtual void onTileCollision(const Tile& tile, const CollisionType& type) = 0;
	virtual void setAirborne(const bool airborne) = 0;

	virtual bool isAirborne();
	virtual bool collidable();
	virtual void despawn();
	std::string name;

	const std::string& getName() const;


	virtual sf::FloatRect getPosAdjustedAABB();


	void setPosX(const float newx);

	void setPosY(const float newy);

	void setVelX(const float newVelX);

	void setVelY(const float newVelY);

	void moveX(const float motionX);

	void moveY(const float motionY);

	void move(const float motionX, const float motionY);

	float getPosX();

	float getPosY();

	float getVelX();

	float getVelY();


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

