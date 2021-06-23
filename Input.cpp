#include "stdafx.h"
#include "Input.h"
#include "Window.h"

KeyboardInput::KeyboardInput() {
	inputMap.emplace(InputButton::UP, (int)sf::Keyboard::W);
	inputMap.emplace(InputButton::DOWN, (int)sf::Keyboard::S);
	inputMap.emplace(InputButton::LEFT, (int)sf::Keyboard::A);
	inputMap.emplace(InputButton::RIGHT, (int)sf::Keyboard::D);
	inputMap.emplace(InputButton::JUMP, (int)sf::Keyboard::Space);
}

void KeyboardInput::update(Window& window) {
	pressedKeys.clear();
}

bool KeyboardInput::handleWindowEvent(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		pressedKeys[(int)event.key.code] = event.key.code;
		//return false;
	}

	return false;
}

bool KeyboardInput::isDown(const InputButton key) const {
	return pressedKeys.find(inputMap.at(key)) != pressedKeys.end();
}

bool KeyboardInput::isPressed(const InputButton key) const {
	return sf::Keyboard::isKeyPressed((sf::Keyboard::Key)inputMap.at(key));
}
	
	bool KeyboardInput::isDown(sf::Keyboard::Key key) const {
		return sf::Keyboard::isKeyPressed(key);
	}
	
	
	bool KeyboardInput::isPressed(sf::Keyboard::Key key) const {
		return pressedKeys.find((int)key) != pressedKeys.end();
	}