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
			//updateTurn();
			break;
		case ActionState::Airborne:
			//updateAirborne();
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

		while (listener->events.size() > 0) {
			auto event = listener->events.front();
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

			listener->events.pop();
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

		while (listener->events.size() > 0) {
			auto event = listener->events.front();
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

			listener->events.pop();
		}



		updateAirborneTimer(entry, state);
	}

	void updateAirborne(std::shared_ptr<ActorEntry> entry, StateController* state) {

		auto listener = scene->getComponent<TileCollisionEventListener>(entry);

		auto input = scene->getComponent<InputController>(entry)->input;
		auto vel = scene->getComponent<Velocity>(entry);

		bool right = input->isDown(InputButton::RIGHT);
		bool left = input->isDown(InputButton::LEFT);

		while (listener->events.size() > 0) {
			auto event = listener->events.front();
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

			listener->events.pop();
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