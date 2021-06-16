#pragma once
#include "Actor.h"
#include "Animation.h"

class ActorData;

class Boop :
	public Actor
{
public:
	Boop(const ActorData& data);

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

