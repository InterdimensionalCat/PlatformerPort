#pragma once
#include "Texture.h"
class Input;
class Animation;

enum class ActionState {
	GroundStill,
	GroundMove,
	GroundTurn,
	Airborne
};

struct PlayerFlag {
	bool player = true;
};

struct StateController {
	ActionState state;
};

struct InputController {
	Input* input;
};

struct Animatable {
	std::shared_ptr<Animation> animation;
};

struct Transform {
	float x;
	float y;
};

struct Velocity {
	float x;
	float y;
};

struct BoundedVelocity {
	float maxVelX;
	float maxVelY;
};

struct PhysicsProperties {
	float friction;
	float gravity;
	float airResistance;
};

struct Jumpable {
	float jumpForce;
	float jumpReleaseMod;
};

struct Turnable {
	float turnThreshold;
};

struct AirborneState {
	int airborneTimer;
	int maxAirborneTimer;
};

struct HorzMovable {
	float accelX;
};

struct ConstrainedVerticalMovement {
	float maxDisplacementUp;
	float maxDisplacementDown;
};

struct ConstrainedHorizontalMovement {
	float maxDisplacementLeft;
	float maxDisplacementRight;
};

struct JumpOn {
	float jumpTolerencePercent;
	float jumpVelMod;
	float jumpMin;
};

struct SpriteDrawable {
	sf::Sprite* spr;
	Texture* tex;

	~SpriteDrawable() {
		delete spr;
		delete tex;
	}
};

struct Hitbox {
	sf::FloatRect rect;
};

struct HitboxExtension {
	float extensionUp;
	float extensionDown;

	float extensionLeft;
	float extensionRight;
};

struct TileCollidable {
	bool tileCollidable = true;
};

