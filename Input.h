#pragma once
#include "WindowEventListener.h"

class Window;

class Input;
class KeyboardInput;

typedef std::function<std::shared_ptr<Input>(const std::string&)> InputFinder;

extern std::map<std::string, InputFinder> findInput;
extern std::shared_ptr<KeyboardInput> keyboard;

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
	Input(const std::string &typeName, InputFinder finderfunc) {
		findInput.emplace(typeName, finderfunc);
	}

	virtual bool isDown(const InputButton key) const = 0;
	virtual bool isPressed(const InputButton key) const = 0;
	virtual std::string getName() const = 0;
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
	std::string getName() const override;

private:
	std::map<int, sf::Keyboard::Key> pressedKeys;
	std::map<InputButton, int> inputMap;
};

