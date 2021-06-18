#pragma once

#include "Window.h"
#include "MapParser.h"

class Tilemap;
class Parallax;
class Camera;
class KeyboardInput;
class PhysicsEngine;
class AudioEngine;
class Actor;

class Scene
{
public:
	Scene();

	~Scene();

	void update();

	void removeActor(Actor* actor);

	void cleanupActors();

	void draw(const float interpol);

	void setChangeLevel();

	void changeLevel();

	void setResetLevel();

	void resetLevel();

	std::shared_ptr<AudioEngine> audio;

private:

	friend class LevelData;
	friend class Player;
	friend class MapParser;

	void init();

	//tilemap
	std::shared_ptr<Tilemap> tilemap;
	//actors
	//std::shared_ptr<Player> player;
	//camera
	std::unique_ptr<Camera> camera;
	//background layers
	std::unique_ptr<Parallax> parallaxEngine;

	std::shared_ptr<PhysicsEngine> engine;

	std::vector<std::shared_ptr<Actor>> actors;

	std::vector<Actor*> removeActors;

	std::shared_ptr<Window> window;

	std::shared_ptr<KeyboardInput> input;

	MapParser parser;

	bool changeLevelFlag = false;
	bool resetLevelFlag = false;

	int currentLevel = 1;

	std::string levelBaseName = "newLevel";
};

