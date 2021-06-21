#include "stdafx.h"
#include "PlayerLogic.h"
#include "Input.h"
#include "Player.h"
#include "LevelData.h"
#include "Scene.h"
#include "AudioEngine.h"

PlayerLogic::PlayerLogic(const ActorData& data) {

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

	updateSFX();
}

void PlayerLogic::updateTurn() {
	//try horz move
	switch (moveHorz(groundAccel * 2.0f)) {
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

	if (input->isDown(sf::Keyboard::LShift)) {
		jump(jumpForce);
	}

	checkSpeedBounds(maxSpeedX, maxSpeedY);
}

void PlayerLogic::shortenJump() {
	if (!jumpFlag) return;
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
	if (player->getVelY() > 0.0f) {
		jumpFlag = false;
	}
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

	auto jforce = jumpForce;

	if(input->isDown(sf::Keyboard::LShift)) {
		jforce *= 3.0f;
	}

	if (input->isPressed(sf::Keyboard::Space)) {
		move(0, -jforce);

		jumpFlag = true;


		if (player->scene->audio.getStatusOfSound("PlayerJump1") != sf::SoundSource::Status::Playing) {
			player->scene->audio.playSound("PlayerJump1", 12.0f);
		}
		else {
			player->scene->audio.playSound("PlayerJump2", 12.0f);
		}

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

void PlayerLogic::updateSFX() {
	if (player->state == ActionState::GroundMove) {
		groundedAudioTimer++;
		if (player->vel.x != 0) {
			int cycle = (int)round(60.0f / (abs(player->vel.x) * audioCycleMod));

			if (cycle > 45) cycle = 45;
			if (cycle < 10) cycle = 10;

			if (groundedAudioTimer >= cycle) {
				if (stepFlag) {
					player->scene->audio.playSound("PlayerDash1", 10.0f);
				}
				else {
					player->scene->audio.playSound("PlayerDash2", 10.0f);
				}
				groundedAudioTimer = 0;
				stepFlag = !stepFlag;
			}
		}
	}
}