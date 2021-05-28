#pragma once
#include "Window.h"
#include "LevelData.h"

enum class CameraMode {
	Controlled,
	Follow,
	Stationary
};

class Camera
{
public:
	Camera(const LevelData& data, const CameraMode& mode = CameraMode::Controlled) : mode(mode) {
		boundsInPixels = sf::Vector2f(toPixels(data.levelWidth), toPixels(data.levelHeight));
	}
	void updateWindow(ic::Window& window) {
		switch (mode) {
		case CameraMode::Controlled:
			controlled(window);
			break;
		case CameraMode::Follow:
			follow(window);
			break;
		case CameraMode::Stationary:
			stationary(window);
			break;
		}
	}

	void reset(ic::Window& window, const sf::Vector2f pixelResetPos = sf::Vector2f(0, 0)) {
		auto resetView = window.window->getDefaultView();
		resetView.move(pixelResetPos);
		window.window->setView(resetView);
	}

	void changeViewMode(const CameraMode& newmode, sf::Vector2f newpos, ic::Window& window) {
		reset(window, newpos);
		mode = newmode;
	}

	void checkBounds() {
		if (scrollpos.x + (float)Settings::getSetting<int>("Width") > boundsInPixels.x) {
			scrollpos.x = boundsInPixels.x - (float)Settings::getSetting<int>("Width");
		}

		if (scrollpos.y + (float)Settings::getSetting<int>("Height") > boundsInPixels.y) {
			scrollpos.y = boundsInPixels.y - (float)Settings::getSetting<int>("Height");
		}

		if (scrollpos.x < 0) {
			scrollpos.x = 0;
		}

		if (scrollpos.y < 0) {
			scrollpos.y = 0;
		}

	}

	void controlled(ic::Window& window) {

		window.window->setView(window.window->getDefaultView());
		auto newview = window.window->getView();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			scrollpos -= sf::Vector2f(5, 0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			scrollpos += sf::Vector2f(0, 5);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			scrollpos += sf::Vector2f(5, 0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			scrollpos -= sf::Vector2f(0, 5);
		}

		checkBounds();

		newview.move(scrollpos);

		window.window->setView(newview);
	}

	void follow(ic::Window& window) {

	}

	void stationary(ic::Window& window) {

	}

	CameraMode mode;
	sf::Vector2f scrollpos;
	sf::Vector2f boundsInPixels;
};

