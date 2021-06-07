#pragma once


namespace ic {
	class Window;
}

class LevelData;
class Actor;

enum class CameraMode {
	Controlled,
	Follow,
	Stationary
};

class Camera
{
public:

	Camera(const LevelData& data, std::shared_ptr<Actor> followTarget);

	Camera(const LevelData& data, const CameraMode& mode = CameraMode::Controlled);
	void updateWindow(ic::Window& window);

	void reset(ic::Window& window, const sf::Vector2f pixelResetPos = sf::Vector2f(0, 0));

	void changeViewMode(const CameraMode& newmode, sf::Vector2f newpos, ic::Window& window);

	void checkBounds();

	void controlled(ic::Window& window);

	void follow(ic::Window& window);

	void stationary(ic::Window& window);

	std::shared_ptr<Actor> followTarget;

	CameraMode mode;
	sf::Vector2f scrollpos;
	sf::Vector2f boundsInPixels;
};

