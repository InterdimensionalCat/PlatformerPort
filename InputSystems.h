#pragma once
#include "System.h"
#include "input.h"


class HorzMove : public System<Velocity, HorzMovable, InputController> {
public:
	HorzMove(Scene* scene) : System(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto vel = scene->getComponent<Velocity>(entry);
		auto accelX = scene->getComponent<HorzMovable>(entry)->accelX;
		auto input = scene->getComponent<InputController>(entry)->input;

		bool right = input->isDown(InputButton::RIGHT);
		bool left = input->isDown(InputButton::LEFT);
		
		if ((right && left) || (!right && !left)) {
			return;
		}
		if (right) {
			vel->x += accelX;
		}
		if (left) {
			vel->x -= accelX;
		}
	}
};

class UpdateFrcByState : public System<PhysicsProperties, StateController, StateFrictionValues> {
public:
	UpdateFrcByState(Scene* scene) : System(scene) {}

	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto frc = scene->getComponent<PhysicsProperties>(entry);
		auto frcVals = scene->getComponent<StateFrictionValues>(entry);
		auto currentstate = scene->getComponent<StateController>(entry);

		switch (currentstate->state) {
		case ActionState::Airborne:
			frc->friction = frcVals->airFrc;
			break;
		case ActionState::GroundStill:
			frc->friction = frcVals->stillFrc;
			break;
		case ActionState::GroundMove:
			frc->friction = frcVals->groundFrc;
			break;
		case ActionState::GroundTurn:
			frc->friction = frcVals->turnFrc;
			break;
		}
	}
};

class UpdateAccelByState : public System<HorzMovable, StateController, StateAccelerationValues> {
public:
	UpdateAccelByState(Scene* scene) : System(scene) {}

	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto accelX = scene->getComponent<HorzMovable>(entry);
		auto accelVals = scene->getComponent<StateAccelerationValues>(entry);
		auto currentstate = scene->getComponent<StateController>(entry);

		switch (currentstate->state) {
		case ActionState::Airborne:
			accelX->accelX = accelVals->airAccel;
			break;
		case ActionState::GroundStill:
			accelX->accelX = accelVals->stillAccel;
			break;
		case ActionState::GroundMove:
			accelX->accelX = accelVals->groundAccel;
			break;
		case ActionState::GroundTurn:
			accelX->accelX = accelVals->turnAccel;
			break;
		}
	}
};

class GenericStateDriver : public System<StateController, InputController, AirborneState, TileCollisionEventListener, Velocity> {
public:
	GenericStateDriver(Scene* scene) : System(scene) {}

	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto statecontroller = scene->getComponent<StateController>(entry);
		auto stateForFrame = statecontroller->state;

		switch (stateForFrame) {
		case ActionState::GroundStill:
			updateStill(entry, statecontroller);
			break;
		case ActionState::GroundMove:
			updateRun(entry, statecontroller);
			break;
		case ActionState::GroundTurn:
			updateTurn(entry, statecontroller);
			break;
		case ActionState::Airborne:
			updateAirborne(entry, statecontroller);
			break;
		}
	}
private:
	void updateStill(std::shared_ptr<ActorEntry> entry, StateController* state) {

		auto listener = scene->getComponent<TileCollisionEventListener>(entry);

		auto input = scene->getComponent<InputController>(entry)->input;
		auto vel = scene->getComponent<Velocity>(entry);

		bool right = input->isDown(InputButton::RIGHT);
		bool left = input->isDown(InputButton::LEFT);

		if ((right && left) || (!right && !left)) {
			state->state = ActionState::GroundStill;
		}
		else {
			if (abs(vel->x) > toMeters(0.5f)) {
				state->state = ActionState::GroundMove;
			}
		}

		while (listener->events->size() > 0) {
			auto event = listener->events->front();
			switch (event.face) {
			case CollisionFace::WallLeft:
			case CollisionFace::WallRight:
				state->state = ActionState::GroundStill;
				break;
			case CollisionFace::Ceiling:
				break;
			case CollisionFace::Floor:
			    {
				    auto airbornetimer = scene->getComponent<AirborneState>(entry);
				    airbornetimer->airborneTimer = airbornetimer->maxAirborneTimer;
			    }
			}

			listener->events->pop_front();
		}



		updateAirborneTimer(entry, state);
	}

	void updateRun(std::shared_ptr<ActorEntry> entry, StateController* state) {

		auto listener = scene->getComponent<TileCollisionEventListener>(entry);

		auto input = scene->getComponent<InputController>(entry)->input;
		auto vel = scene->getComponent<Velocity>(entry);

		bool right = input->isDown(InputButton::RIGHT);
		bool left = input->isDown(InputButton::LEFT);

		if ((right && left) || (!right && !left)) {
			if (abs(vel->x) < toMeters(0.5f)) {
				state->state = ActionState::GroundStill;
			}
		} else {
			if ((right && vel->x < 0) || (left && vel->x > 0)) {
				auto turnable = scene->getComponent<Turnable>(entry);
				if (turnable != nullptr) {
					if (abs(vel->x) > turnable->turnThreshold) {
						state->state = ActionState::GroundTurn;
					}
				}
			}
		}

		while (listener->events->size() > 0) {
			auto event = listener->events->front();
			switch (event.face) {
			case CollisionFace::WallLeft:
			case CollisionFace::WallRight:
				state->state = ActionState::GroundStill;
				break;
			case CollisionFace::Ceiling:
				break;
			case CollisionFace::Floor:
			{
				auto airbornetimer = scene->getComponent<AirborneState>(entry);
				airbornetimer->airborneTimer = airbornetimer->maxAirborneTimer;
			}
			}

			listener->events->pop_front();
		}



		updateAirborneTimer(entry, state);
	}

	void updateTurn(std::shared_ptr<ActorEntry> entry, StateController* state) {

		auto listener = scene->getComponent<TileCollisionEventListener>(entry);

		auto input = scene->getComponent<InputController>(entry)->input;
		auto vel = scene->getComponent<Velocity>(entry);

		bool right = input->isDown(InputButton::RIGHT);
		bool left = input->isDown(InputButton::LEFT);

		if ((right && left) || (!right && !left)) {
			if (abs(vel->x) < toMeters(0.5f)) {
				state->state = ActionState::GroundStill;
			}
		}
		else {
			if ((right && vel->x > 0) || (left && vel->x < 0)) {
				//done moving into the turn
				state->state = ActionState::GroundMove;
			}
		}

		while (listener->events->size() > 0) {
			auto event = listener->events->front();
			switch (event.face) {
			case CollisionFace::WallLeft:
			case CollisionFace::WallRight:
				state->state = ActionState::GroundStill;
				break;
			case CollisionFace::Ceiling:
				break;
			case CollisionFace::Floor:
			{
				auto airbornetimer = scene->getComponent<AirborneState>(entry);
				airbornetimer->airborneTimer = airbornetimer->maxAirborneTimer;
			}
			}

			listener->events->pop_front();
		}



		updateAirborneTimer(entry, state);
	}

	void updateAirborne(std::shared_ptr<ActorEntry> entry, StateController* state) {

		auto listener = scene->getComponent<TileCollisionEventListener>(entry);

		auto input = scene->getComponent<InputController>(entry)->input;
		auto vel = scene->getComponent<Velocity>(entry);

		bool right = input->isDown(InputButton::RIGHT);
		bool left = input->isDown(InputButton::LEFT);

		while (listener->events->size() > 0) {
			auto event = listener->events->front();
			switch (event.face) {
			case CollisionFace::WallLeft:
				break;
			case CollisionFace::WallRight:
				break;
			case CollisionFace::Ceiling:
				break;
			case CollisionFace::Floor:
			{
				auto airbornetimer = scene->getComponent<AirborneState>(entry);
				airbornetimer->airborneTimer = airbornetimer->maxAirborneTimer;
				if (abs(vel->x) < toMeters(0.5f)) {
					state->state = ActionState::GroundStill;
				}
				else {
					state->state = ActionState::GroundMove;
				}

			}
			}

			listener->events->pop_front();
		}



		updateAirborneTimer(entry, state);
	}

	void updateAirborneTimer(std::shared_ptr<ActorEntry> entry, StateController* state) {
		auto airbornetimer = scene->getComponent<AirborneState>(entry);
		airbornetimer->airborneTimer--;
		if (airbornetimer->airborneTimer <= 0) {
			airbornetimer->airborneTimer = 0;
			state->state = ActionState::Airborne;
		}
	}
};

class JumpInput : public System<InputController, StateController, Velocity, Jumpable, PhysicsProperties> {
public:
	JumpInput(Scene* scene) : System(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto input = scene->getComponent<InputController>(entry)->input;
		auto vel = scene->getComponent<Velocity>(entry);
		auto statecontroller = scene->getComponent<StateController>(entry);
		auto jump = scene->getComponent<Jumpable>(entry);
		auto properties = scene->getComponent<PhysicsProperties>(entry);

		if (statecontroller->state == ActionState::Airborne) {

			//shorten jump
			if (jump->jumpedflag) {
				if (!input->isDown(InputButton::JUMP)) {
					if (vel->y < 0) {
						vel->y *= jump->jumpReleaseMod;
					}
				}
				else {
					if (vel->y > toMeters(0.5f) && vel->y < toMeters(3.0f)) {
						vel->y -= properties->gravity * jump->jumpFloat;
					}

					if (vel->y < -toMeters(0.5f) && vel->y > -toMeters(3.0f)) {
						vel->y -= properties->gravity * jump->jumpFloat;
					}
				}
			}


		} else {
			if (vel->y == 0) {
				jump->jumpedflag = false;
			}

			//try jump
			if (input->isPressed(InputButton::JUMP)) {
				vel->y -= jump->jumpForce;
				jump->jumpedflag = true;
			}
		}
	}
};

class CheckPlayerFallOffMap : public System<MapBoundCollisionListener, PlayerFlag> {
public:
	CheckPlayerFallOffMap(Scene* scene) : System(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto listener = scene->getComponent<MapBoundCollisionListener>(entry);
		while (!listener->events->empty()) {
			auto event = listener->events->front();

			if (event == MapBoundCollisionEvent::HitDown) {
				scene->pushEvent<ResetLevelEvent>();
			}

			listener->events->pop_front();
		}
	}
};