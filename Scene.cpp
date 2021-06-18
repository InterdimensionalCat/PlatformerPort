#include "stdafx.h"
#include "Scene.h"
#include "Tilemap.h"
#include "TileRegistry.h"
#include "Window.h"
#include "Parallax.h"
#include "Camera.h"
#include "Input.h"
#include "PhysicsEngine.h"
#include "AnimationRegistry.h"
#include "AudioRegistry.h"
#include "AudioEngine.h"
#include "Actor.h"

Scene::Scene() {
	init();
	window = std::make_shared<Window>();
	input = std::make_shared<KeyboardInput>();
	window->registerWindowEventListener(input);

	parser.parseMap(levelBaseName + "_" + std::to_string(currentLevel), *this);


	engine = std::make_shared<PhysicsEngine>(this);

	audio = std::make_shared<AudioEngine>();
	audio->playMusic("FlatZone", 10.0f);
}

Scene::~Scene() {
	actors.clear();
	clearAnimRegistry();
	clearAudioRegistry();
}

void Scene::update() {
	input->update(*window);

	if (!window->updateInput()) {
		Settings::setSetting<bool>("running", false);
		return;
	}

	for (auto& actor : actors) {
		actor->update();
	}


	engine->update(actors, *tilemap);

	camera->updateWindow(*window);
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

	window->preRender(interpol);

	parallaxEngine->draw(*window);
	tilemap->draw(*window);
	//player->draw(window);
	for (const auto& actor : actors) {
		actor->draw(*window);
	}

	window->postRender();
}

void Scene::setChangeLevel() {
	audio->playSound("PlayerWin", 20.0f);
	changeLevelFlag = true;
}

void Scene::changeLevel() {
	changeLevelFlag = false;

	currentLevel++;
	
	fs::path filepath(fs::current_path());
	filepath /= "resources";
	filepath /= "maps";

	if (fs::exists(filepath / (levelBaseName + "_" + std::to_string(currentLevel) + ".json"))) {
		parser.parseMap(levelBaseName + "_" + std::to_string(currentLevel), *this);
	}
	else {
		Settings::setSetting<bool>("running", false);
		return;
	}
}

void Scene::setResetLevel() {
	resetLevelFlag = true;
}

void Scene::resetLevel() {
	resetLevelFlag = false;
	parser.parseMap(levelBaseName + "_" + std::to_string(currentLevel), *this);
}



void Scene::init() {
	RegisterTiles();
	RegisterAnimations();
	RegisterAudio();
}
