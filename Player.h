#pragma once
#include "Actor.h"
#include "LevelData.h"
#include "PlayerLogic.h"
#include "PlayerGraphics.h"
#include "Scene.h"

enum class ActionState {
	GroundStill,
	GroundMove,
	GroundTurn,
	Airborne
};

class Player : public Actor
{
public:
	Player(const LevelData& data) : Actor("Player"),
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

	void despawn() override {
		scene->audio->playSound("PlayerDead", 15.0f);
		scene->setResetLevel();
	}

	void update() override {
		logic->update();
		graphics->update();
	}

	void draw(ic::Window& window) const override {
		graphics->draw(window);
	}

	void changeState(const ActionState& newstate) {

		if (state == newstate) return;
		state = newstate;
		graphics->changeState(newstate);

		if (state == ActionState::GroundStill) {
			//logic->groundedAudioTimer = 0;
			//logic->stepFlag = true;
		}
	}

	void setAirborne(const bool airborne) override {
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

	bool isAirborne() override { return state == ActionState::Airborne; }

	void onCollision(std::shared_ptr<Actor> actor) override {}

	void onTileCollision(const Tile& tile, const CollisionType& type) override {
		if (type == CollisionType::Floor) {
			setAirborne(false);
		}
	}

	mutable std::unique_ptr<PlayerGraphics> graphics;
	std::unique_ptr<PlayerLogic> logic;
	ActionState state = ActionState::GroundStill;
};

