#pragma once

class Scene;
class ActorEntry;
class Window;

enum class CameraMode {
	Controlled,
	Follow,
	Stationary
};

class Camera
{
public:

	Camera() {}

	Camera(const float width, const float height, std::shared_ptr<ActorEntry> followtarget, Scene* scene);

	Camera(const float width, const float height, const CameraMode& mode = CameraMode::Controlled);
	void updateWindow(Window& window);

	void reset(Window& window, const sf::Vector2f pixelResetPos = sf::Vector2f(0, 0));

	void changeViewMode(const CameraMode& newmode, sf::Vector2f newpos, Window& window);

	void checkBounds();

	void controlled(Window& window);

	void follow(Window& window);

	void stationary(Window& window);

	std::shared_ptr<ActorEntry> followTarget;

	CameraMode mode;
	sf::Vector2f scrollpos;
	sf::Vector2f boundsInPixels;
	Scene* scene;
};

