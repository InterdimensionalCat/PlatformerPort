#pragma once
#include "Component.h"
#include "LevelData.h"
#include "Player.h"

class PlayerLogic
	: public LogicComponent
{
	PlayerLogic(const LevelData& data) : LogicComponent(data) {}

	virtual void update() override {

	}

	float maxSpeedX = toMeters(6.0f);
	float groundAccel = toMeters(0.3f);
	float airAccel = toMeters(3.0f);
	float jumpForce = toMeters(15.0f);
	float jumpReleaseSpeedMod = 2.0f; // divide this by 2
	ActionState state = ActionState::GroundStill;
};

