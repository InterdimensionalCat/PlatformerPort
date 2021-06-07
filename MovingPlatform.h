#pragma once
#include "Actor.h"
#include "Texture.h"

class Scene;
class LevelData;
//namespace ic {
//	Window;
//}

enum class PlatformType {
	Vertical,
	Horizontal,
	Falling
};

class MovingPlatform :
	public Actor
{
public:

	MovingPlatform(const LevelData& data, const float spawnX, const float spawnY, const float range, const PlatformType type);

	sf::FloatRect getPosAdjustedAABB() override;

	sf::FloatRect getPosAdjustedAABBNoExtention();

	void despawn() override;

	void update() override;

	void draw(ic::Window& window) const override;

	void moveHorz();

	void moveVert();

	void moveFalling();

	void onCollision(std::shared_ptr<Actor> actor) override;

	void onTileCollision(const Tile& tile, const CollisionType& type) override;

	void setAirborne(const bool airborne) override;

	bool collidable() override;

	ic::Texture tex;
	sf::Sprite spr;

	sf::Vector2f spawnpos;
	float range;

	float speedX = toMeters(1.5f) * 2.0f;
	float speedY = toMeters(2.0f) * 2.0f;

	float grv = toMeters(0.5);
	float maxFallingSpeed = toMeters(25.0f);
	bool falling = false;
	PlatformType type;

	float platformTolerenceUp = toMeters(4.0f);
	float platformTolerenceDown = toMeters(20.0f);

	//float yHitboxExtention = toMeters(20.0f);

	float yHitboxExtention = toMeters(40.0f);

	int signbit = 0;
};

