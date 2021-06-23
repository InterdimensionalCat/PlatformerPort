#pragma once
#include "Texture.h"
#include "TileCollisionEvent.h"

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
	std::shared_ptr<Input> input;
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
	std::shared_ptr<sf::Sprite> spr;
	std::shared_ptr<Texture> tex;
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


struct TileCollisionEventListener {
	std::queue<TileCollisionEvent> events;
};

