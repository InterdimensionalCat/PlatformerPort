#include "stdafx.h"
#include "Input.h"
#include "Window.h"

//std::map<std::string, InputFinder> Input::prototypeTable;

KeyboardInput::KeyboardInput() {
	inputMap.emplace(InputButton::UP,    sf::Keyboard::W    );
	inputMap.emplace(InputButton::DOWN,  sf::Keyboard::S    );
	inputMap.emplace(InputButton::LEFT,  sf::Keyboard::A    );
	inputMap.emplace(InputButton::RIGHT, sf::Keyboard::D    );
	inputMap.emplace(InputButton::JUMP,  sf::Keyboard::Space);
}

bool KeyboardInput::isDown(const InputButton key) const {
	return handle.isDown(inputMap.at(key));
}
	

bool KeyboardInput::isPressed(const InputButton key) const {
	return handle.isPressed(inputMap.at(key));
}