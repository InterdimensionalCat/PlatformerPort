#pragma once
#include "Actor.h"
#include "Animation.h"

class LevelData;

class Boop :
	public Actor
{
public:
	Boop(const LevelData& data, const float spawnX, const float spawnY, const float speedX, const float displacement, const bool jumper, const float grv = toMeters(0.5));

	void update();
	void draw(Window& window) const;
	void onCollision(std::shared_ptr<Actor> actor) override;

	void despawn() override;

	void onTileCollision(const Tile& tile, const CollisionType& type) override;

	void setAirborne(const bool airborne) override;

	bool isAirborne() override;

	void updateAnim();


	float boopKillTolerencePercent = 0.7f;
	//float boopJumpVal = toMeters(25.0f);
	float jumpVelMod = 1.5f;
	//float jumpVelMod = 0.8f;
	float jumpMin = toMeters(10.0f);
	//float jumpMin = toMeters(5.0f);

	bool airborne = true;
	float spawnPointX;
	float speedX;
	float displacement;
	bool jumper;
	mutable Animation anim;

	float jmp = toMeters(15.0f);

	float grv;// = toMeters(0.5);
};

