#include "stdafx.h"
#include "BoopAI.h"


BoopAI::BoopAI() {}

void BoopAI::update() {
	keylistener.update();
}

bool BoopAI::isDown(const InputButton key) const {
	return keylistener.isDown(key);
}

bool BoopAI::isPressed(const InputButton key) const {
	return keylistener.isPressed(key);
}

void BoopAI::pressKey(const InputButton key) {
	keylistener.pressKey(key);
}

void BoopAI::releaseKey(const InputButton key) {
	keylistener.releaseKey(key);
}