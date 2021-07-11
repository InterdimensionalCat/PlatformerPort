#include "stdafx.h"
#include "Input.h"
#include "Window.h"

std::map<std::string, InputFinder>findInput = std::map<std::string, InputFinder>();
std::shared_ptr<KeyboardInput> keyboard = std::make_shared<KeyboardInput>();


std::shared_ptr<Input> findKeyboard(const std::string& name) {
	return keyboard;
}


KeyboardInput::KeyboardInput() : Input("KeyboardInput", findKeyboard)  {
	inputMap.emplace(InputButton::UP, (int)sf::Keyboard::W);
	inputMap.emplace(InputButton::DOWN, (int)sf::Keyboard::S);
	inputMap.emplace(InputButton::LEFT, (int)sf::Keyboard::A);
	inputMap.emplace(InputButton::RIGHT, (int)sf::Keyboard::D);
	inputMap.emplace(InputButton::JUMP, (int)sf::Keyboard::Space);
}

std::string KeyboardInput::getName() const {
	return "KeyboardInput";
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
	return sf::Keyboard::isKeyPressed((sf::Keyboard::Key)inputMap.at(key));
}
	

bool KeyboardInput::isPressed(const InputButton key) const {
	return pressedKeys.find(inputMap.at(key)) != pressedKeys.end();
}
	
bool KeyboardInput::isDown(sf::Keyboard::Key key) const {
	return sf::Keyboard::isKeyPressed(key);
}
	
	
bool KeyboardInput::isPressed(sf::Keyboard::Key key) const {
	return pressedKeys.find((int)key) != pressedKeys.end();
}