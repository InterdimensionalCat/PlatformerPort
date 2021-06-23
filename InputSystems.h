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