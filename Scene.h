#pragma once
#include "Tilemap.h"
#include "LevelRegistry.h"
#include "TIleRegistry.h"
#include "Window.h"
#include "Parallax.h"
#include "Camera.h"
#include "Player.h"
#include "InputHandle.h"
#include "PHysicsEngine.h"
#include "AnimationRegistry.h"

class Scene
{
public:
	Scene() {
		init();
		auto& firstLevel = getLevelData(1);
		tilemap = std::make_shared<Tilemap>(firstLevel);
		parallaxEngine = std::make_unique<Parallax>(firstLevel);
		input = std::make_shared<InputHandle>();
		engine = std::make_shared<PhysicsEngine>();

		actors = firstLevel.loadActors(this);

		camera = std::make_unique<Camera>(firstLevel, actors.at(0));
	}
	void update() {
		//window.updateInput();

		input->updateInput(*window.window);
		//player->update();

		for (auto& actor : actors) {
			actor->update();
		}

		//auto actors = std::vector<std::shared_ptr<Actor>>{ player };

		engine->update(actors, *tilemap);

		camera->updateWindow(window);
		parallaxEngine->move(*camera);

		if (changeLevelFlag) {
			changeLevel();
		}
	}
	void draw(const float interpol) {

		window.preRender(interpol);

		parallaxEngine->draw(window);
		tilemap->draw(window);
		//player->draw(window);
		for (const auto& actor : actors) {
			actor->draw(window);
		}

		window.postRender();
	}

	void setChangeLevel() {
		changeLevelFlag = true;
	}

	void changeLevel() {
		changeLevelFlag = false;
		auto& newLevelData = getLevelData(++currentLevel);

		tilemap = std::make_shared<Tilemap>(newLevelData);
		parallaxEngine = std::make_unique<Parallax>(newLevelData);
		actors = newLevelData.loadActors(this);
		camera = std::make_unique<Camera>(newLevelData, actors.at(0));
	}

private:

	friend class LevelData;

	void init() {
		RegisterTiles();
		RegisterLevels();
		RegisterAnimations();
	}

	//tilemap
	std::shared_ptr<Tilemap> tilemap;
	//actors
	//std::shared_ptr<Player> player;
	//camera
	std::unique_ptr<Camera> camera;
	//background layers
	std::unique_ptr<Parallax> parallaxEngine;

	std::shared_ptr<InputHandle> input;

	std::shared_ptr<PhysicsEngine> engine;

	std::vector<std::shared_ptr<Actor>> actors;

	ic::Window window;

	bool changeLevelFlag = false;

	int currentLevel = 1;
};

