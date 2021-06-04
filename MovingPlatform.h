#pragma once
#include "Actor.h"
#include "Texture.h"
#include "Scene.h"

enum class PlatformType {
	Vertical,
	Horizontal,
	Falling
};

class MovingPlatform :
	public Actor
{
public:

	MovingPlatform(const LevelData& data, const float spawnX, const float spawnY, const float range, const PlatformType type)
		: Actor("MovingPlatform"), range(range), type(type), tex("movingTile") {

		scene = data.scene;

		pos = sf::Vector2f(spawnX, spawnY);

		spawnpos = pos;
		spr.setTexture(tex.getTexture());
		spr.setPosition(toPixels(pos.x), toPixels(pos.y));
		switch (type) {
		case PlatformType::Vertical:
			vel = sf::Vector2f(0, speedY);
			hitbox = sf::FloatRect(0, 0, toMeters(128.0f), toMeters(20.0f));
			break;
		case PlatformType::Horizontal:
			vel = sf::Vector2f(speedX, 0);
			hitbox = sf::FloatRect(0, 0, toMeters(128.0f), toMeters(20.0f));
			break;
		case PlatformType::Falling:
			vel = sf::Vector2f(0, 0);
			hitbox = sf::FloatRect(0, 0, toMeters(128.0f), toMeters(20.0f));
			break;
		}
	}

	sf::FloatRect getPosAdjustedAABB() override {
		return sf::FloatRect(hitbox.left + pos.x, (hitbox.top + pos.y) - yHitboxExtention, hitbox.width, hitbox.height + yHitboxExtention);
	}

	sf::FloatRect getPosAdjustedAABBNoExtention() {
		return sf::FloatRect(hitbox.left + pos.x, hitbox.top + pos.y, hitbox.width, hitbox.height);
	}

	void despawn() override {
		scene->removeActor(this);
	}

	void update() override {
		switch (type) {
		case PlatformType::Vertical:
			moveVert();
			break;
		case PlatformType::Horizontal:
			moveHorz();
			break;
		case PlatformType::Falling:
			moveFalling();
			break;
		}

		spr.setPosition(toPixels(pos.x), toPixels(pos.y));
	}

	void draw(ic::Window& window) const override {
		window.window->draw(spr);
	}

	void moveHorz() {
		auto min = spawnpos.x;
		auto max = spawnpos.x + range;
		if (pos.x + speedX >= max) {
			vel.x =  -speedX;
			signbit = -1;
		}

		if (pos.x - speedX <= min) {
			vel.x = speedX;
			signbit = 1;
		}
	}

	void moveVert() {
		auto min = spawnpos.y;
		auto max = spawnpos.y + range;
		if (pos.y + speedY >= max) {
			vel.y = -speedY;
			signbit = -1;
		}

		if (pos.y - speedY <= min) {
			vel.y = speedY;
			signbit = 1;
		}

	}

	void moveFalling() {
		if (falling) {
			vel.y += grv;
			if (vel.y > maxFallingSpeed) {
				vel.y = maxFallingSpeed;
			}
		}
	}

	void onCollision(std::shared_ptr<Actor> actor) override {
		if (actor->getName() == "Player" || actor->getName() == "Boop") {


			auto hitbox = actor->getPosAdjustedAABB();
			hitbox.left += actor->getVelX();
			hitbox.top += actor->getVelY();

			auto bottomCorner = hitbox.top + hitbox.height;
			//auto topCorner = hitbox.top

			auto hitboxPlatform = getPosAdjustedAABBNoExtention();
			hitboxPlatform.left += getVelX();
			hitboxPlatform.top += getVelY();

			auto topCornerPlatform = hitboxPlatform.top;


			//cases where player should not stick to the platform
			if (actor->isAirborne()) {
				//narrow phase check
				if (!hitbox.intersects(hitboxPlatform)) {
					return;
				}
			}
			else {
				//if actor is grounded it was probably on the platform to begin with
				//but just in case the player's y pos must be higher than the true hitbox
				if (hitbox.top > hitboxPlatform.top) return;
			}

			if (bottomCorner - platformTolerenceDown < topCornerPlatform && actor->getVelY() > 0) {
				//collided
				actor->setPosY(hitboxPlatform.top - hitbox.height);
				if (type == PlatformType::Horizontal) {
					if (actor->isAirborne()) {
						actor->setVelX(0.0f);
					}
					actor->setVelY(0.0f);
					actor->setPosX(actor->getPosX() + speedX * (float)signbit);
				}
				if (type == PlatformType::Vertical) {
					actor->setVelY(0.0f);
					if (signbit == -1) {
						//actor->setPosY(actor->getPosY() + speedY * (float)signbit + platformTolerenceUp);
						//actor->setPosY(actor->getPosY() + speedY);
					}
					else {
						//actor->setPosY(actor->getPosY() + speedY * (float)signbit);
					}
					actor->setPosY(actor->getPosY() + speedY - platformTolerenceUp);
				}
				if (type == PlatformType::Falling) {
					actor->setVelY(grv);
					//actor->setPosY(actor->getPosY() + grv + platformTolerenceUp * 2.0f);
					actor->setPosY(actor->getPosY() + grv - platformTolerenceUp);
					//actor->setPosY(actor->getPosY() + grv);
				}

				actor->setAirborne(false);

				if (type == PlatformType::Falling) {
					falling = true;
				}
			}


		}
	}

	void onTileCollision(const Tile& tile, const CollisionType& type) override {}

	void setAirborne(const bool airborne) override {

	}

	bool collidable() override { return false; }

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

