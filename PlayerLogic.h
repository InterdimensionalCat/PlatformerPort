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
	//void updatePos();
	bool applyFriction(const float frc);
	void shortenJump();
	void applyGravity();
	void checkSpeedBounds(const float maxX, const float maxY);
	void updateAirborneTimer();

	void updateSFX();

	//float maxSpeedX = toMeters(6.0f);
	float maxSpeedX = toMeters(6.2f);
	//float maxSpeedY = toMeters(25.0f);
	 float maxSpeedY = toMeters(100.0f);
	//float maxSpeedX = toMeters(4.0f);
	float groundAccel = toMeters(0.3f);
	//float groundAccel = toMeters(3.3f);
	//float airAccel = toMeters(3.0f);
	float airAccel = toMeters(1.0f);
	float jumpForce = toMeters(15.0f);
	float turnThreshold = toMeters(2.0f);
	//float jumpReleaseSpeedMod = 2.0f; // divide this by 2
	float jumpReleaseSpeedMod = 1.2f; // divide this by 2
	//float frictionConstant = 1.4f; //divisor
	//float frictionConstant = 1.03f; //divisor
	float frictionConstant = 1.05f; //divisor
	float gravity = toMeters(0.5f);
	//float gravity = toMeters(0.1f);
	float airResistance = 1.008f;

	int airborneTimer = 0;
	int maxAirborneTimer = 5;

	int groundedAudioTimer = 0;
	float audioCycleMod = 60.0f;
	bool stepFlag = true;

	bool jumpFlag = false;

	//sf::FloatRect hitbox = sf::FloatRect(0, 0, toMeters(64 - 25), toMeters(64 - 10));
	//sf::Vector2f pos = sf::Vector2f(0, 0);
	//sf::Vector2f vel = sf::Vector2f(0, 0);

	Player* player;
	std::shared_ptr<InputHandle> input;
};

