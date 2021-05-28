#pragma once
#include "Actor.h"
#include "LevelData.h"
#include "PlayerLogic.h"
#include "PlayerPhysics.h"
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
	Player(const LevelData& data) : Actor("Player",
		std::make_unique<PlayerGraphics>(data),
		std::make_unique<PlayerPhysics>(data),
		std::make_unique<PlayerLogic>(data)
	) {}

	void update() {
		logic->update();
		graphics->update();
	}

	void draw(ic::Window& window) const {
		graphics->draw(window);
	}

	std::unique_ptr<PlayerGraphics> graphics;
	std::unique_ptr<PlayerLogic> logic;
};

