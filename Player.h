#pragma once
#include "Actor.h"
#include "PlayerGraphics.h"

namespace ic {
	class Window;
}

class LevelData;
class PlayerGraphics;
class PlayerLogic;

enum class ActionState {
	GroundStill,
	GroundMove,
	GroundTurn,
	Airborne
};

class Player : public Actor
{
public:
	Player(const LevelData& data);

	void despawn() override;

	void update() override;

	void draw(ic::Window& window) const override;

	void changeState(const ActionState& newstate);

	void setAirborne(const bool airborne) override;

	bool isAirborne() override;

	void onCollision(std::shared_ptr<Actor> actor) override;

	void onTileCollision(const Tile& tile, const CollisionType& type);

	mutable std::unique_ptr<PlayerGraphics> graphics;
	std::unique_ptr<PlayerLogic> logic;
	ActionState state = ActionState::GroundStill;
};

