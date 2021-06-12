#pragma once
#include "WindowEventListener.h"

class Window;

enum class InputButton {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	JUMP
};

class Input
{
public:
	Input() {}
	virtual bool isDown(const InputButton key) const = 0;
	virtual bool isPressed(const InputButton key) const = 0;
};

class KeyboardInput : public Input, public WindowEventListener {
public:
	KeyboardInput();

    bool handleWindowEvent(const sf::Event& event) override;

	bool isDown(const InputButton key) const override;
	bool isPressed(const InputButton key) const override;

	bool isDown(sf::Keyboard::Key key) const;
	bool isPressed(sf::Keyboard::Key key) const;

    void update(Window& window) override;

private:
	std::map<int, sf::Keyboard::Key> pressedKeys;
	std::map<InputButton, int> inputMap;
};

