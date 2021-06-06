#pragma once
#include "Actor.h"
#include "LevelData.h"
#include "Animation.h"
#include "AnimationRegistry.h"
#include "Scene.h"

class Boop :
	public Actor
{
public:
	Boop(const LevelData& data, const float spawnX, const float spawnY, const float speedX, const float displacement, const bool jumper, const float grv = toMeters(0.5)) 
		: Actor("Boop"), spawnPointX(spawnX), speedX(speedX), displacement(displacement), jumper(jumper), anim(getAnimFromName("BoopAnim")), grv(grv) {
		pos = sf::Vector2f(spawnX, spawnY);
		hitbox = sf::FloatRect(0, 0, 1.0f, 1.0f);
		scene = data.scene;
	}

	void update() {
		if (abs(pos.x - spawnPointX) >  displacement) {
			speedX = -speedX;
		}

		setVelX(speedX);

		vel.y += grv;

		if (!isAirborne() && jumper) {
			setVelY(-jmp);
			setAirborne(true);
		}

		updateAnim();
	}
	void draw(ic::Window& window) const {
		anim.draw(window);
	}
	void onCollision(std::shared_ptr<Actor> actor) override {
		if (actor->getName() == "Player") {
			auto playerY = actor->getPosY();
			auto boopY = getPosY();

			if (boopY - playerY > getPosAdjustedAABB().height * boopKillTolerencePercent) {
				//kills boop
				this->despawn();

				auto playervely = actor->getVelY();

				if (playervely > 0) {
					playervely *= -1.0f;
				}
				auto jumpForce = std::min(playervely * jumpVelMod, -jumpMin);

				//actor->setVelY(actor->getVelY() - boopJumpVal);
				actor->setVelY(jumpForce);
			}
			else {
				//kills player
				actor->despawn();
			}
		}
	}

	void despawn() override {

		if (scene->audio->getStatusOfSound("BoopDeath") != sf::SoundSource::Status::Playing) {
			scene->audio->playSound("BoopDeath", 15.0f);
		}
		else {
			scene->audio->playSound("BoopDeath2", 15.0f);
		}

		scene->removeActor(this);
	}

	void onTileCollision(const Tile& tile, const CollisionType& type) override {
		if (type == CollisionType::Wall) {
			speedX = -speedX;
		}

		if (type == CollisionType::Floor) {
			setAirborne(false);
		}
	}

	void setAirborne(const bool airborne) override {
		this->airborne = airborne;
	}

	bool isAirborne() override { return airborne; }

	void updateAnim() {
		auto hitbox = getPosAdjustedAABB();
		auto width = toMeters(anim.targetDim.x);
		auto height = toMeters(anim.targetDim.y);

		auto center = sf::Vector2f(hitbox.left + hitbox.width / 2.0f, hitbox.top + hitbox.height / 2.0f);

		anim.setPositition(sf::Vector2f(toPixels(center.x - width / 2.0f), toPixels(hitbox.top + hitbox.height - height)));
	}


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

