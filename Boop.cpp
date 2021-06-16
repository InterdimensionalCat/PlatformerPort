#include "stdafx.h"
#include "Boop.h"
#include "LevelData.h"
#include "AnimationRegistry.h"
#include "Scene.h"
#include "AudioEngine.h"
#include "ActorData.h"


Boop::Boop(const ActorData& data)
	: Actor("Boop"), anim(getAnimFromName("BoopAnim")) {
	auto spawnX = toMeters(data.findKey<float>("x"));
	auto spawnY = toMeters(data.findKey<float>("y")) - 1.0f;

	auto DisplacementLeft = toMeters(data.findKey<float>("DisplacementLeft"));
	auto DisplacementRight = toMeters(data.findKey<float>("DisplacementRight"));

	grv = toMeters(data.findKey<float>("gravity"));

	jumper = data.findKey<bool>("jumper");

	speedX = toMeters(data.findKey<float>("speed"));

	scene = data.scene;

	auto diff = DisplacementRight - DisplacementLeft;

	spawnPointX = spawnX + diff / 2.0f;
	displacement = (DisplacementRight + DisplacementLeft) / 2.0f;
	pos = sf::Vector2f(spawnX, spawnY);
	hitbox = sf::FloatRect(0, 0, 1.0f, 1.0f);
	scene = data.scene;
}

void Boop::update() {
	if (abs(pos.x - spawnPointX) > displacement) {
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
void Boop::draw(Window& window) const {
	anim.draw(window);
}
void Boop::onCollision(std::shared_ptr<Actor> actor) {
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

void Boop::despawn() {

	if (scene->audio->getStatusOfSound("BoopDeath") != sf::SoundSource::Status::Playing) {
		scene->audio->playSound("BoopDeath", 15.0f);
	}
	else {
		scene->audio->playSound("BoopDeath2", 15.0f);
	}

	scene->removeActor(this);
}

void Boop::onTileCollision(const Tile& tile, const CollisionType& type) {
	if (type == CollisionType::Wall) {
		speedX = -speedX;
	}

	if (type == CollisionType::Floor) {
		setAirborne(false);
	}
}

void Boop::setAirborne(const bool airborne) {
	this->airborne = airborne;
}

bool Boop::isAirborne() { return airborne; }

void Boop::updateAnim() {
	auto hitbox = getPosAdjustedAABB();
	auto width = toMeters(anim.targetDim.x);
	auto height = toMeters(anim.targetDim.y);

	auto center = sf::Vector2f(hitbox.left + hitbox.width / 2.0f, hitbox.top + hitbox.height / 2.0f);

	anim.setPositition(sf::Vector2f(toPixels(center.x - width / 2.0f), toPixels(hitbox.top + hitbox.height - height)));
}