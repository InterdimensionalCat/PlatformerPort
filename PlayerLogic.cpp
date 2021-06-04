#include "stdafx.h"
#include "PlayerLogic.h"
#include "InputHandle.h"
#include "Player.h"

PlayerLogic::PlayerLogic(const LevelData& data) {

}

void PlayerLogic::update() {
	auto stateForFrame = player->state;
	applyGravity();

	switch (stateForFrame) {
	case ActionState::GroundStill:
		updateStill();
		break;
	case ActionState::GroundMove:
		updateRun();
		break;
	case ActionState::GroundTurn:
		updateTurn();
		break;
	case ActionState::Airborne:
		updateAirborne();
		break;
	}

	//updatePos();
}

void PlayerLogic::updateStill() {

	//try horz move
	if (moveHorz(groundAccel * 3) != MoveResult::Still) {
		player->changeState(ActionState::GroundMove);
	}
	else {
		applyFriction(frictionConstant);
	}

	//try jump
	if (jump(jumpForce)) {
		player->changeState(ActionState::Airborne);
	}

	updateAirborneTimer();


}

void PlayerLogic::updateRun() {
	//try horz move
	switch(moveHorz(groundAccel)) {
	case MoveResult::Left:

		//vel is opposite the move dir and high -> turn state
		if (player->vel.x > 0) {
			if (abs(player->vel.x) > turnThreshold) {
				player->changeState(ActionState::GroundTurn);
			}
			else {
				player->vel.x = -groundAccel;
			}
		}
		break;
	case MoveResult::Right:

		//vel is opposite the move dir and high -> turn state
		if (player->vel.x < 0) {
			if (abs(player->vel.x) > turnThreshold) {
				player->changeState(ActionState::GroundTurn);
			}
			else {
				player->vel.x = groundAccel;
			}
		}
		break;

	case MoveResult::Still:

		if (!applyFriction(frictionConstant)) {
			player->changeState(ActionState::GroundStill);
		}
		break;
	}

	//try jump
	if (jump(jumpForce)) {
		player->changeState(ActionState::Airborne);
	}

	updateAirborneTimer();

	checkSpeedBounds(maxSpeedX, maxSpeedY);
}

void PlayerLogic::updateTurn() {
	//try horz move
	switch (moveHorz(groundAccel)) {
	case MoveResult::Left:

		//vel is in the opposite direction of the turn
		if (player->vel.x < 0) {
			player->changeState(ActionState::GroundMove);
		}
		break;
	case MoveResult::Right:

		//vel is in the opposite direction of the turn
		if (player->vel.x > 0) {
			player->changeState(ActionState::GroundMove);
		}
		break;
	case MoveResult::Still:

		if (!applyFriction(frictionConstant)) {
			player->changeState(ActionState::GroundStill);
		}
		break;
	}

	//try jump
	if (jump(jumpForce)) {
		player->changeState(ActionState::Airborne);
	}

	updateAirborneTimer();

	checkSpeedBounds(maxSpeedX, maxSpeedY);
}

void PlayerLogic::updateAirborne() {
	//try move
	if (moveHorz(airAccel) == MoveResult::Still) {
		applyFriction(airResistance);
	}

	//shorten jump
	shortenJump();

	checkSpeedBounds(maxSpeedX, maxSpeedY);
}

void PlayerLogic::shortenJump() {
	if (!input->isDown(sf::Keyboard::Space)) {
		if (player->vel.y < 0) {
			player->vel.y /= jumpReleaseSpeedMod;
		}
	}
}

void PlayerLogic::updateAirborneTimer() {
	if (--airborneTimer <= 0) {
		airborneTimer = 0;
		player->changeState(ActionState::Airborne);
	}
}

void PlayerLogic::applyGravity() {
	move(0.0f, gravity);
}

void PlayerLogic::checkSpeedBounds(const float maxX, const float maxY) {
	if (player->vel.x >  maxX) player->vel.x =  maxX;
	if (player->vel.x < -maxX) player->vel.x = -maxX;
	if (player->vel.y >  maxY) player->vel.y =  maxY;
	if (player->vel.y < -maxY) player->vel.y = -maxY;
}

bool PlayerLogic::applyFriction(const float frc) {
	player->vel.x /= frc;
	if (abs(player->vel.x) < toMeters(1)) {
		player->vel.x = 0;
		return false;
	}
	else {
		return true;
	}
}

MoveResult PlayerLogic::moveHorz(const float AccelX) {
	if (input->isDown(sf::Keyboard::D)) {
		move(AccelX, 0);
		return MoveResult::Right;
	}

	if (input->isDown(sf::Keyboard::A)) {
		move(-AccelX, 0);
		return MoveResult::Left;
	}

	return MoveResult::Still;
}

bool PlayerLogic::jump(const float jumpForce) {
	if (input->isPressed(sf::Keyboard::Space)) {
		move(0, -jumpForce);
		return true;
	}
	return false;
}

void PlayerLogic::move(float motionX, float motionY) {
	player->vel += sf::Vector2f(motionX, motionY);
}
//void PlayerLogic::updatePos() {
//	pos += vel;
//}