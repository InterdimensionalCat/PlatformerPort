#include "stdafx.h"
#include "InputHandle.h"

bool InputHandle::updateInput(sf::RenderWindow& window) {

	//clear pressed keys from the previous update
	pressedKeys.clear();
	pressedButtons.clear();


	//poll an event

	//window events for main window;

	sf::Event event;
	while (window.pollEvent(event))
	{

		//events that should be polled regardless of window active state:
		if (event.type == sf::Event::Closed) {
			return false;
		}


		if (event.type == sf::Event::KeyPressed) {
			pressedKeys[(int)event.key.code] = event.key.code;

			if (event.key.code == sf::Keyboard::I) {
				Settings::setSetting<bool>("debug", !Settings::getSetting<bool>("debug"));
			}

			if (event.key.code == sf::Keyboard::Escape) {
				return false;
			}

		}

		if (event.type == sf::Event::MouseButtonPressed) {
			pressedButtons[(int)event.mouseButton.button] = event.mouseButton.button;
		}
	}

	return true;
}
	
	bool InputHandle::isDown(sf::Keyboard::Key key) const {
		return sf::Keyboard::isKeyPressed(key);
	}
	
	bool InputHandle::isDown(sf::Mouse::Button key) const {
		return sf::Mouse::isButtonPressed(key);
	}
	
	bool InputHandle::isPressed(sf::Keyboard::Key key) const {
		return pressedKeys.find((int)key) != pressedKeys.end();
	}
	
	bool InputHandle::isPressed(sf::Mouse::Button key) const {
		return pressedButtons.find((int)key) != pressedButtons.end();
	}