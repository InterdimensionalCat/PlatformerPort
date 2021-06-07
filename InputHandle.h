#pragma once

class InputHandle
{
public:
	InputHandle() {}
	~InputHandle() {}

	//determine if a key is held down
	bool isDown(sf::Keyboard::Key key) const;

	//determine if a mouse button is held down
	bool isDown(sf::Mouse::Button key) const;

	//determine if a key was pressed on this update
	bool isPressed(sf::Keyboard::Key key) const;

	//determine if a mouse button was pressed on this update
	bool isPressed(sf::Mouse::Button key) const;

	//read in all input given on this frame
	bool updateInput(sf::RenderWindow& window);

	//map containing all the keys
	std::map<int, sf::Keyboard::Key > pressedKeys;
	std::map<int, sf::Mouse::Button> pressedButtons;



};

