#pragma once
#include "Tilemap.h"
#include "LevelRegistry.h"
#include "TIleRegistry.h"
#include "Window.h"
#include "Parallax.h"
#include "Camera.h"
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

		actors = firstLevel.loadActors(this);

		engine = std::make_shared<PhysicsEngine>(firstLevel);

		camera = std::make_unique<Camera>(firstLevel, actors.at(0));
	}
	void update() {
		//window.updateInput();

		if (!input->updateInput(*window.window)) {
			Settings::setSetting<bool>("running", false);
		}
		//player->update();

		for (auto& actor : actors) {
			actor->update();
		}

		//auto actors = std::vector<std::shared_ptr<Actor>>{ player };

		engine->update(actors, *tilemap);

		camera->updateWindow(window);
		parallaxEngine->move(*camera);

		if (input->isPressed(sf::Keyboard::R)) {
			setResetLevel();
		}

		if (input->isPressed(sf::Keyboard::Num1)) {
			setChangeLevel();
		}

		cleanupActors();

		if (changeLevelFlag) {
			changeLevel();
		}

		if (resetLevelFlag) {
			resetLevel();
		}
	}

	void removeActor(Actor* actor) {
		removeActors.push_back(actor);
	}

	void cleanupActors() {

		for (auto actor : removeActors) {
			for (size_t i = 0; i < actors.size(); i++) {
				if (actors.at(i).get() == actor) {
					actors.erase(actors.begin() + i);
					continue;
				}
			}
		}

		removeActors.clear();
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

		if (currentLevel + 1 > numLevels) {
			Settings::setSetting<bool>("running", false);
			return;
		}

		auto& newLevelData = getLevelData(++currentLevel);

		tilemap = std::make_shared<Tilemap>(newLevelData);
		parallaxEngine = std::make_unique<Parallax>(newLevelData);
		actors = newLevelData.loadActors(this);
		camera = std::make_unique<Camera>(newLevelData, actors.at(0));
	}

	void setResetLevel() {
		resetLevelFlag = true;
	}

	void resetLevel() {
		resetLevelFlag = false;

		auto& newLevelData = getLevelData(currentLevel);

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

	std::vector<Actor*> removeActors;

	ic::Window window;

	bool changeLevelFlag = false;
	bool resetLevelFlag = false;

	int currentLevel = 1;

	int numLevels = 3;
};

