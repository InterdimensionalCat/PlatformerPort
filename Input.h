#pragma once
#include "WindowEventListener.h"

class Window;

class Input;
class KeyboardInput;

typedef std::function<std::shared_ptr<Input>(const std::string&)> InputFinder;

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

	virtual void update() = 0;
	virtual bool isDown(const InputButton key) const = 0;
	virtual bool isPressed(const InputButton key) const = 0;
	virtual std::string getName() const = 0;

	virtual void pressKey(const InputButton key) {}
	virtual void releaseKey(const InputButton key) {}

	static std::shared_ptr<Input> createInputDevice(const std::string& devicename) {
		return prototypeTable.at(devicename)(devicename);
	}

protected:
	static void addPrototype(const std::string& type, InputFinder finder) {
		prototypeTable.insert(std::pair(type, finder));
	}
	inline static std::map<std::string, InputFinder> prototypeTable;
};

#ifndef _Input_Device
#define _Input_Device(classtype) public:\
static std::shared_ptr<Input> create(const std::string& name) {\
    return std::make_shared<classtype>();\
}\
std::string getName() const override { return #classtype; }\
private:\
class StartupObj {\
public:\
		StartupObj() {\
			Input::addPrototype( #classtype, create); \
	}\
}; \
friend class StartupObj; \
inline static StartupObj startup;
#endif

template<typename KeyType>
class KeyListener {
public:
	bool isDown(KeyType key) const {
		return hkeys.find((int)key) != hkeys.end();
	}

	bool isPressed(KeyType key) const {
		return pkeys.find((int)key) != pkeys.end();
	}

	void update() {
		pkeys.clear();
	}

	void pressKey(const KeyType key) {
		pkeys.emplace((int)key, key);
		hkeys.emplace((int)key, key);
	}

	void releaseKey(const KeyType key) {
		hkeys.erase((int)key);
	}

	std::map<int, KeyType> pkeys = std::map<int, KeyType>();
	std::map<int, KeyType> hkeys = std::map<int, KeyType>();
};

class KeyboardGlobal {
public:

private:
	friend class KeyboardHandle;
	friend class Window;

	class KeyboardGlobalEventListener : public WindowEventListener {
	public:

		bool handleWindowEvent(const sf::Event& event) {
			if (event.type == sf::Event::KeyPressed) {
				pressKey(event.key.code);
			}

			if (event.type == sf::Event::KeyReleased) {
				releaseKey(event.key.code);
			}

			return false;
		}

		void update(Window& window) {
			keylistener.update();
		}

		void pressKey(const sf::Keyboard::Key key) {
			keylistener.pressKey(key);
		}

		void releaseKey(const sf::Keyboard::Key key) {
			keylistener.releaseKey(key);
		}

		KeyListener<sf::Keyboard::Key> keylistener;
	};

	static bool isDown(sf::Keyboard::Key key) {
		return listener->keylistener.isDown(key);
	}

	static bool isPressed(sf::Keyboard::Key key) {
		return listener->keylistener.isPressed(key);
	}

	static inline std::shared_ptr<KeyboardGlobalEventListener> listener
		= std::make_shared<KeyboardGlobalEventListener>();
};

class KeyboardHandle {
public:
	bool isDown(sf::Keyboard::Key key) const {
		return KeyboardGlobal::isDown(key);
	}
	bool isPressed(sf::Keyboard::Key key) const {
		return KeyboardGlobal::isPressed(key);
	}
};


class KeyboardInput : public Input {
public:
	KeyboardInput();

	void update() override {}
	bool isDown(const InputButton key) const override;
	bool isPressed(const InputButton key) const override;

	_Input_Device(KeyboardInput);

//public:
//	static std::shared_ptr<Input> create(const std::string& name) {
//		return std::make_shared<KeyboardInput>();
//	}
//	std::string getName() const override { return "KeyboardInput"; }
//private:
//	class StartupObj {
//	public:
//		StartupObj() {
//			Input::addPrototype("KeyboardInput", create);
//		}
//	};
//	friend class StartupObj;
//	inline static StartupObj startup;


private:
	KeyboardHandle handle;
	std::map<InputButton, sf::Keyboard::Key> inputMap;
};

