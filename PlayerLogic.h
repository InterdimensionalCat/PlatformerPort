#pragma once
#include "LevelData.h"

class Player;
class InputHandle;

enum class MoveResult {
	Still,
	Left,
	Right
};

class PlayerLogic
{
public:
	PlayerLogic(const LevelData& data);
	void update();
	void updateStill();
	void updateRun();
	void updateTurn();
	void updateAirborne();
	MoveResult moveHorz(const float AccelX);
	bool jump(const float jumpForce);
	void move(float motionX, float motionY);
	void updatePos();
	bool applyFriction(const float frc);
	void shortenJump();
	void applyGravity();
	void checkSpeedBounds(const float maxX, const float maxY);

	float maxSpeedX = toMeters(6.0f);
	float groundAccel = toMeters(0.3f);
	float airAccel = toMeters(3.0f);
	float jumpForce = toMeters(15.0f);
	float turnThreshold = toMeters(3.0f);
	float jumpReleaseSpeedMod = 2.0f; // divide this by 2
	float frictionConstant = 1.4f; //divisor
	float gravity = toMeters(0.5f);
	float airResistance = 1.02f;

	sf::FloatRect hitbox = sf::FloatRect(0, 0, toMeters(64 - 25), toMeters(64 - 10));
	sf::Vector2f pos = sf::Vector2f(0, 0);
	sf::Vector2f vel = sf::Vector2f(0, 0);

	Player* player;
	std::shared_ptr<InputHandle> input;
};

