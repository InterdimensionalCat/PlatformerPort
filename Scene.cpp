#include "stdafx.h"
#include "Scene.h"
#include "Tilemap.h"
#include "LevelRegistry.h"
#include "TileRegistry.h"
#include "Window.h"
#include "Parallax.h"
#include "Camera.h"
#include "InputHandle.h"
#include "PhysicsEngine.h"
#include "AnimationRegistry.h"
#include "AudioRegistry.h"
#include "AudioEngine.h"
#include "Actor.h"

Scene::Scene() {
	init();
	auto& firstLevel = getLevelData(1);
	tilemap = std::make_shared<Tilemap>(firstLevel);
	parallaxEngine = std::make_unique<Parallax>(firstLevel);
	input = std::make_shared<InputHandle>();

	actors = firstLevel.loadActors(this);

	engine = std::make_shared<PhysicsEngine>(firstLevel);

	camera = std::make_unique<Camera>(firstLevel, actors.at(0));

	audio = std::make_shared<AudioEngine>();
	audio->playMusic("FlatZone", 10.0f);
}

Scene::~Scene() {
	clearAnimRegistry();
	clearAudioRegistry();
}

void Scene::update() {
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

void Scene::removeActor(Actor* actor) {
	removeActors.push_back(actor);
}

void Scene::cleanupActors() {

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

void Scene::draw(const float interpol) {

	window.preRender(interpol);

	parallaxEngine->draw(window);
	tilemap->draw(window);
	//player->draw(window);
	for (const auto& actor : actors) {
		actor->draw(window);
	}

	window.postRender();
}

void Scene::setChangeLevel() {
	audio->playSound("PlayerWin", 20.0f);
	changeLevelFlag = true;
}

void Scene::changeLevel() {
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

void Scene::setResetLevel() {
	resetLevelFlag = true;
}

void Scene::resetLevel() {
	resetLevelFlag = false;

	auto& newLevelData = getLevelData(currentLevel);

	tilemap = std::make_shared<Tilemap>(newLevelData);
	parallaxEngine = std::make_unique<Parallax>(newLevelData);
	actors = newLevelData.loadActors(this);
	camera = std::make_unique<Camera>(newLevelData, actors.at(0));
}



void Scene::init() {
	RegisterTiles();
	RegisterLevels();
	RegisterAnimations();
	RegisterAudio();
}
