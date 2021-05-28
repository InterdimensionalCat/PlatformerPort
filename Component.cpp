#include "stdafx.h"
#include "Window.h"
#include "InputHandle.h"
#include "Actor.h"

Component::Component(const LevelData& data) {

}

void Component::setActor(Actor* actor) {
	owner = actor;
}

void LogicComponent::moveHorz(const float AccelX) {
	if (input->isDown(sf::Keyboard::A)) {
		owner->physics->move(-AccelX, 0);
	}

	if (input->isDown(sf::Keyboard::A)) {
		owner->physics->move(AccelX, 0);
	}
}

void LogicComponent::jump(const float jumpForce) {
	owner->physics->move(0, -jumpForce);
}