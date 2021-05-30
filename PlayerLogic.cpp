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

	updatePos();
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


}

void PlayerLogic::updateRun() {
	//try horz move
	switch(moveHorz(groundAccel)) {
	case MoveResult::Left:

		//vel is opposite the move dir and high -> turn state
		if (vel.x > 0) {
			if (abs(vel.x) > turnThreshold) {
				player->changeState(ActionState::GroundTurn);
			}
			else {
				vel.x = -groundAccel;
			}
		}
		break;
	case MoveResult::Right:

		//vel is opposite the move dir and high -> turn state
		if (vel.x < 0) {
			if (abs(vel.x) > turnThreshold) {
				player->changeState(ActionState::GroundTurn);
			}
			else {
				vel.x = groundAccel;
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
}

void PlayerLogic::updateTurn() {
	//try horz move
	switch (moveHorz(groundAccel)) {
	case MoveResult::Left:

		//vel is in the opposite direction of the turn
		if (vel.x < 0) {
			player->changeState(ActionState::GroundMove);
		}
		break;
	case MoveResult::Right:

		//vel is in the opposite direction of the turn
		if (vel.x > 0) {
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
}

void PlayerLogic::updateAirborne() {
	//try move
	if (moveHorz(airAccel) == MoveResult::Still) {
		applyFriction(airResistance);
	}

	//shorten jump
	shortenJump();

	//check if still airborn
}

void PlayerLogic::shortenJump() {
	if (!input->isDown(sf::Keyboard::Space)) {
		if (vel.y < 0) {
			vel.y /= jumpReleaseSpeedMod;
		}
	}
}

void PlayerLogic::applyGravity() {
	move(0.0f, gravity);
}

void PlayerLogic::checkSpeedBounds(const float maxX, const float maxY) {
	if (vel.x >  maxX) vel.x =  maxX;
	if (vel.x < -maxX) vel.x = -maxX;
	if (vel.y >  maxY) vel.y =  maxY;
	if (vel.y < -maxY) vel.y = -maxY;
}

bool PlayerLogic::applyFriction(const float frc) {
	vel.x /= frc;
	if (abs(vel.x) < toMeters(2)) {
		vel.x = 0;
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
	vel += sf::Vector2f(motionX, motionY);
}
void PlayerLogic::updatePos() {
	pos += vel;
}