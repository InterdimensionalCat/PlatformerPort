#pragma once
#include "Actor.h"
#include "LevelData.h"
#include "PlayerLogic.h"
#include "PlayerGraphics.h"

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
		graphics->player = this;
		logic->player = this;
		hitbox = sf::FloatRect(0, 0, toMeters(64 - 25), toMeters(64 - 10));
		pos = sf::Vector2f(0, 0);
		vel = sf::Vector2f(0, 0);
	}

	void update() override {
		logic->update();
		graphics->update();
	}

	void draw(ic::Window& window) const override {
		graphics->draw(window);
	}

	void changeState(const ActionState& newstate) {
		state = newstate;
	}

	std::unique_ptr<PlayerGraphics> graphics;
	std::unique_ptr<PlayerLogic> logic;
	ActionState state = ActionState::GroundStill;
};

