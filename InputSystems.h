#pragma once
#include "System.h"
#include "input.h"


class HorzMove : public SystemTrivial<Velocity, HorzMovable, InputController> {
public:
	HorzMove(Scene* scene) : SystemTrivial(scene) {}
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

	_System(HorzMove, SystemType::PrePhysics);
};

class UpdateFrcByState : public SystemTrivial<PhysicsProperties, StateController, StateFrictionValues> {
public:
	UpdateFrcByState(Scene* scene) : SystemTrivial(scene) {}

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

	_System(UpdateFrcByState, SystemType::PostLogic);
};

class UpdateAccelByState : public SystemTrivial<HorzMovable, StateController, StateAccelerationValues> {
public:
	UpdateAccelByState(Scene* scene) : SystemTrivial(scene) {}

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

	_System(UpdateAccelByState, SystemType::PostLogic);
};

class GenericStateDriver : public SystemTrivial<StateController, InputController, AirborneState, TileCollisionEventListener, Velocity> {
public:
	GenericStateDriver(Scene* scene) : SystemTrivial(scene) {}

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

	_System(GenericStateDriver, SystemType::Logic);
};

class JumpInput : public SystemTrivial<InputController, StateController, Velocity, Jumpable, PhysicsProperties> {
public:
	JumpInput(Scene* scene) : SystemTrivial(scene) {}
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

	_System(JumpInput, SystemType::PrePhysics);
};

class CheckPlayerFallOffMap : public SystemTrivial<MapBoundCollisionListener, PlayerFlag> {
public:
	CheckPlayerFallOffMap(Scene* scene) : SystemTrivial(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto listener = scene->getComponent<MapBoundCollisionListener>(entry);
		while (!listener->events->empty()) {
			auto event = listener->events->front();

			if (event == MapBoundCollisionEvent::HitDown) {
				playerDeath(entry);
			}

			listener->events->pop_front();
		}
	}

	void playerDeath(std::shared_ptr<ActorEntry> entry) {
		scene->pushEvent<ResetLevelEvent>();
	}

	_System(CheckPlayerFallOffMap, SystemType::PostUpdate);

};

class PlatformDespawn : public SystemTrivial<MapBoundCollisionListener, PlatformTypeable> {
public:
	PlatformDespawn(Scene* scene) : SystemTrivial(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto listener = scene->getComponent<MapBoundCollisionListener>(entry);
		while (!listener->events->empty()) {
			auto event = listener->events->front();

			if (event == MapBoundCollisionEvent::HitDown) {
				scene->pushEvent<DespawnActorEvent>(entry);
			}

			listener->events->pop_front();
		}
	}

	_System(PlatformDespawn, SystemType::PostUpdate);

};

class ConstrainMovement : public SystemTrivial<Transform, ConstrainedMovement> {
public:
	ConstrainMovement(Scene* scene) : SystemTrivial(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto trans = scene->getComponent<Transform>(entry);
		auto centerX = trans->x;
		auto centerY = trans->y;
		auto hitbox = scene->getComponent<Hitbox>(entry);
		if (hitbox != nullptr) {
			centerX = trans->x + hitbox->rect.width / 2.0f;
			centerY = trans->y + hitbox->rect.height / 2.0f;
		}
		auto constrainedmvmt = scene->getComponent<ConstrainedMovement>(entry);

		if (!(constrainedmvmt->maxDisplacementLeft == 0.0f && constrainedmvmt->maxDisplacementRight == 0.0f)) {
			if (centerX < constrainedmvmt->originX) {
				if (constrainedmvmt->maxDisplacementLeft >= 0.0f
					&& abs(centerX - constrainedmvmt->originX) > constrainedmvmt->maxDisplacementLeft) {
					constrainedmvmt->events->push(ConstrainedMovementEvent::OOBLeft);
				}
			}
			else {
				if (constrainedmvmt->maxDisplacementRight >= 0.0f
					&& abs(centerX - constrainedmvmt->originX) > constrainedmvmt->maxDisplacementRight) {
					constrainedmvmt->events->push(ConstrainedMovementEvent::OOBRight);
				}
			}
		}

		if (!(constrainedmvmt->maxDisplacementUp == constrainedmvmt->maxDisplacementDown == 0.0f)) {
			if (centerY < constrainedmvmt->originY) {
				if (constrainedmvmt->maxDisplacementUp >= 0.0f
					&& abs(centerY - constrainedmvmt->originY) > constrainedmvmt->maxDisplacementUp) {
					constrainedmvmt->events->push(ConstrainedMovementEvent::OOBTop);
				}
			}
			else {
				if (constrainedmvmt->maxDisplacementDown >= 0.0f
					&& abs(centerY - constrainedmvmt->originY) > constrainedmvmt->maxDisplacementDown) {
					constrainedmvmt->events->push(ConstrainedMovementEvent::OOBBot);
				}
			}
		}
	}

	_System(ConstrainMovement, SystemType::PostUpdate);
};

class BoopAiUpdate : public SystemTrivial<EnemyFlag, InputController, ConstrainedMovement, StateController> {
public:
	BoopAiUpdate(Scene* scene) : SystemTrivial(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		if (scene->getComponent<EnemyFlag>(entry)->enemyname != "Boop") return;
		auto constrainedmvmt = scene->getComponent<ConstrainedMovement>(entry);
		auto input = scene->getComponent<InputController>(entry)->input;
		auto state = scene->getComponent<StateController>(entry)->state;
		while (!constrainedmvmt->events->empty()) {
			auto& event = constrainedmvmt->events->front();

			input->update();

			switch (event) {
			case ConstrainedMovementEvent::OOBLeft:
				input->releaseKey(InputButton::LEFT);
				input->pressKey(InputButton::RIGHT);
				break;
			case ConstrainedMovementEvent::OOBRight:
				input->releaseKey(InputButton::RIGHT);
				input->pressKey(InputButton::LEFT);
				break;
			case ConstrainedMovementEvent::OOBTop:
				break;
			case ConstrainedMovementEvent::OOBBot:
				break;
			default:
				break;
			}

			constrainedmvmt->events->pop();
		}

		auto cmvmt = scene->getComponent<ConstrainedMovement>(entry);

		if (!input->isDown(InputButton::RIGHT) && !input->isDown(InputButton::LEFT) && (cmvmt->maxDisplacementLeft > 0.0f || cmvmt->maxDisplacementRight > 0.0f)) {
			input->pressKey(InputButton::RIGHT);
		}

		auto jumper = scene->getComponent<Jumpable>(entry);
		if (jumper != nullptr) {
			//boop is a jumper boop
			if (state != ActionState::Airborne) {
				input->pressKey(InputButton::JUMP);
			}
			else {
				input->releaseKey(InputButton::JUMP);
			}
		}
	}

	_System(BoopAiUpdate, SystemType::Ai);
};

class CircularMovableUpdate : public SystemTrivial<CircularConstrainedMovement, CircularMovable, Velocity, Transform, ConstrainedMovementEventListener> {
public:
	CircularMovableUpdate(Scene* scene) : SystemTrivial(scene) {}

	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto vel   = scene->getComponent<Velocity>(entry);
		auto cmvmt = scene->getComponent<CircularConstrainedMovement>(entry);
		auto cmove = scene->getComponent<CircularMovable>(entry);
		auto trans = scene->getComponent<Transform>(entry);
		auto events = scene->getComponent<ConstrainedMovementEventListener>(entry)->events;

		vel->x += cmove->accel * cmove->angleX;
		vel->y += cmove->accel * cmove->angleY;

		auto nposx = trans->x + vel->x;
		auto nposy = trans->y + vel->y;

		if (cmvmt->radius >= 0.0f) {
			if (dist(nposx, cmvmt->originX, nposy, cmvmt->originY) >= cmvmt->radius) {
				//auto nposx1 = cmvmt->originX + cmvmt->radius * cmove->angleX;
				//auto nposy1 = cmvmt->originY + cmvmt->radius * cmove->angleY;

				//auto nposx2 = cmvmt->originX - cmvmt->radius * cmove->angleX;
				//auto nposy2 = cmvmt->originY - cmvmt->radius * cmove->angleY;
				//vel->x = std::min(nposx1 - trans->x, nposx2 - trans->x);
				//vel->y = std::min(nposy1 - trans->y, nposy2 - trans->y);

				//if (vel->y > 5.0) {
				//	std::cout << "A";
				//}

				events->push(ConstrainedMovementEvent::OOB);
			}
		}

		auto mvmt = scene->getComponent<BoundedVelocity>(entry);

		if (mvmt == nullptr) return;

		if (vel->x > mvmt->maxVelX) {
			vel->x = mvmt->maxVelX;
		}

		if (vel->x < -mvmt->maxVelX) {
			vel->x = -mvmt->maxVelX;
		}

		if (vel->y > mvmt->maxVelY) {
			vel->y = mvmt->maxVelY;
		}

		if (vel->y < -mvmt->maxVelY) {
			vel->y = -mvmt->maxVelY;
		}

	}

	inline float dist(const float x1, const float x2, const float y1, const float y2) const {
		return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	}

	_System(CircularMovableUpdate, SystemType::PrePhysics);
};

class PlatformUpdate : public SystemTrivial<PlatformTypeable, CircularMovable, Velocity, Transform, ConstrainedMovementEventListener> {
public:
	PlatformUpdate(Scene* scene) : SystemTrivial(scene) {}

	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto ptype = scene->getComponent<PlatformTypeable>(entry)->type;
		switch (ptype) {
		case PlatformType::Vertical:
			vert(entry);
			break;
		case PlatformType::Horizontal:
			horz(entry);
			break;
		case PlatformType::Falling:
			falling(entry);
			break;
		case PlatformType::Still:
			break;
		default:
			break;
		}
	}

	void vert(std::shared_ptr<ActorEntry> entry) {
		auto cmove  = scene->getComponent<CircularMovable>(entry);
		auto events = scene->getComponent<ConstrainedMovementEventListener>(entry)->events;
		if (!events->empty()) {
			cmove->angleY *= -1;
		}

		while (!events->empty()) {
			events->pop();
		}
	}

	void horz(std::shared_ptr<ActorEntry> entry) {
		auto cmove = scene->getComponent<CircularMovable>(entry);
		auto events = scene->getComponent<ConstrainedMovementEventListener>(entry)->events;
		if (!events->empty()) {
			cmove->angleX *= -1;
		}

		while (!events->empty()) {
			events->pop();
		}
	}

	void falling(std::shared_ptr<ActorEntry> entry) {
		auto falling = scene->getComponent<FallingPlatform>(entry);
		auto cmove = scene->getComponent<CircularMovable>(entry);
		if (falling->falling) {
			cmove->accel = falling->grv;
			cmove->angleX = 0.0f;
			cmove->angleY = 1.0f;
		}
		else {
			cmove->accel = 0.0f;
			cmove->angleX = 0.0f;
			cmove->angleY = 1.0f;
		}
	}

	_System(PlatformUpdate, SystemType::Logic);
};