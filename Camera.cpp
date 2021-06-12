#include "stdafx.h"
#include "Camera.h"
#include "Window.h"
#include "LevelData.h"
#include "Actor.h"


Camera::Camera(const LevelData& data, std::shared_ptr<Actor> followTarget) : Camera(data, CameraMode::Follow) {
	Camera::followTarget = followTarget;

	auto targetHitbox = followTarget->getPosAdjustedAABB();

	//auto targetScrollPos = sf::Vector2f((scrollpos.x + toPixels(targetHitbox.left + targetHitbox.width)) / 2.0f - (float)Settings::getSetting<int>("Width") / 4.0f,
	//	(scrollpos.y + toPixels(targetHitbox.top + targetHitbox.height)) / 2.0f - (float)Settings::getSetting<int>("Height") / 4.0f);

	auto targetScrollPos = sf::Vector2f(toPixels(targetHitbox.left + targetHitbox.width / 2.0f) - (float)Settings::getSetting<int>("Width") / 2.0f,
		toPixels(targetHitbox.top + targetHitbox.height / 2.0f) - (float)Settings::getSetting<int>("Height") / 2.0f);

	//Logger::get() << targetScrollPos.x << ", " << targetScrollPos.y << "\n";

	scrollpos = targetScrollPos;

	checkBounds();
}

Camera::Camera(const LevelData& data, const CameraMode& mode) : mode(mode) {
	boundsInPixels = sf::Vector2f(toPixels(data.levelWidth), toPixels(data.levelHeight));
}

void Camera::updateWindow(Window& window) {
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

void Camera::reset(Window& window, const sf::Vector2f pixelResetPos) {
	auto resetView = window.window->getDefaultView();
	resetView.move(pixelResetPos);
	window.window->setView(resetView);
}

void Camera::changeViewMode(const CameraMode& newmode, sf::Vector2f newpos, Window& window) {
	reset(window, newpos);
	mode = newmode;
}

void Camera::checkBounds() {
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

void Camera::controlled(Window& window) {

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

void Camera::follow(Window& window) {

	window.window->setView(window.window->getDefaultView());
	auto newview = window.window->getView();

	auto targetHitbox = followTarget->getPosAdjustedAABB();

	auto targetScrollPos = sf::Vector2f((scrollpos.x + toPixels(targetHitbox.left + targetHitbox.width)) / 2.0f - (float)Settings::getSetting<int>("Width") / 4.0f,
		(scrollpos.y + toPixels(targetHitbox.top + targetHitbox.height)) / 2.0f - (float)Settings::getSetting<int>("Height") / 4.0f);

	//auto targetScrollPos = sf::Vector2f(scrollpos.x + toPixels(targetHitbox.left + targetHitbox.width / 2.0f) - (float)Settings::getSetting<int>("Width") / 2.0f,
	//	scrollpos.y + toPixels(targetHitbox.top + targetHitbox.height / 2.0f) - (float)Settings::getSetting<int>("Height") / 2.0f);


	//Logger::get() << targetScrollPos.x << ", " << targetScrollPos.y << "\n";

	auto dist = targetScrollPos -
		scrollpos;


	//Logger::get() << "dist: " << dist.x << ", " << dist.y << "\n";

	//float scrollDivisorX = 1.0f;
	//float scrollDivisorY = 1.0f;


	//float baseDivisor = 60.0f;

	//float maxDist = 200.0f;

	//if (abs(dist.x) < maxDist) {
	//	float scrollDivisorX = baseDivisor - (baseDivisor -1.0f)*(abs(dist.x) / maxDist);
	//}

	//if (abs(dist.y) < maxDist) {
	//	float scrollDivisorY = baseDivisor - (baseDivisor -1.0f)*(abs(dist.y) / maxDist);
	//}

	//scrollpos += dist / 15.0f;

	scrollpos += dist / (15.0f / 2.0f);

	//scrollpos += sf::Vector2f(dist.x / scrollDivisorX, dist.y / scrollDivisorY);

	dist = targetScrollPos -
		scrollpos;

	if (sqrt(dist.x * dist.x + dist.y * dist.y) <= 2.0f) {
		scrollpos = targetScrollPos;
	}


	checkBounds();

	//scrollpos = sf::Vector2f(round(scrollpos.x), round(scrollpos.y));
	scrollpos = sf::Vector2f(scrollpos.x, scrollpos.y);

	newview.move(scrollpos);
	window.window->setView(newview);
}

void Camera::stationary(Window& window) {

}