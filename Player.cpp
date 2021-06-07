#include "stdafx.h"
#include "Player.h"
#include "LevelData.h"
#include "PlayerLogic.h"
#include "PlayerGraphics.h"
#include "Scene.h"
#include "AudioEngine.h"

Player::Player(const LevelData& data) : Actor("Player"),
graphics(std::make_unique<PlayerGraphics>(data)),
logic(std::make_unique<PlayerLogic>(data)) {

	scene = data.scene;

	graphics->player = this;
	logic->player = this;
	hitbox = sf::FloatRect(0, 0, toMeters(64 - 25), toMeters(64 - 10));
	//pos = sf::Vector2f(0, 0);
	pos = data.playerSpawnPos;
	vel = sf::Vector2f(0, 0);
}

void Player::despawn() {
	scene->audio->playSound("PlayerDead", 15.0f);
	scene->setResetLevel();
}

void Player::update() {
	logic->update();
	graphics->update();
}

void Player::draw(ic::Window& window) const {
	graphics->draw(window);
}

void Player::changeState(const ActionState& newstate) {

	if (state == newstate) return;
	state = newstate;
	graphics->changeState(newstate);

	if (state == ActionState::GroundStill) {
		//logic->groundedAudioTimer = 0;
		//logic->stepFlag = true;
	}
}

void Player::setAirborne(const bool airborne) {
	if (airborne) {
		changeState(ActionState::Airborne);
	}
	else {

		logic->airborneTimer = logic->maxAirborneTimer;

		if (state != ActionState::Airborne) return;

		if (abs(getVelX()) > 0) {
			changeState(ActionState::GroundMove);
		}
		else {
			changeState(ActionState::GroundStill);
		}
	}
}

bool Player::isAirborne() { return state == ActionState::Airborne; }

void Player::onCollision(std::shared_ptr<Actor> actor) {}

void Player::onTileCollision(const Tile& tile, const CollisionType& type) {
	if (type == CollisionType::Floor) {
		setAirborne(false);
	}
}
