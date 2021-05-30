#pragma once
#include "Tilemap.h"
#include "LevelRegistry.h"
#include "TIleRegistry.h"
#include "Window.h"
#include "Parallax.h"
#include "Camera.h"
#include "Player.h"
#include "InputHandle.h"

class Scene
{
public:
	Scene() {
		init();
		auto& firstLevel = getLevelData(1);
		tilemap = std::make_unique<Tilemap>(firstLevel);
		parallaxEngine = std::make_unique<Parallax>(firstLevel);
		camera = std::make_unique<Camera>(firstLevel);
		input = std::make_shared<InputHandle>();
		player = std::make_unique<Player>(firstLevel);
		player->logic->input = std::shared_ptr<InputHandle>(input);
	}
	void update() {
		//window.updateInput();

		input->updateInput(*window.window);
		player->update();

		//camera->updateWindow(window);
		parallaxEngine->move(*camera);
	}
	void draw(const float interpol) {

		window.preRender(interpol);

		parallaxEngine->draw(window);
		tilemap->draw(window);
		player->draw(window);

		window.postRender();
	}
private:

	void init() {
		RegisterTiles();
		RegisterLevels();
	}

	//tilemap
	std::unique_ptr<Tilemap> tilemap;
	//actors
	std::unique_ptr<Player> player;
	//camera
	std::unique_ptr<Camera> camera;
	//background layers
	std::unique_ptr<Parallax> parallaxEngine;

	std::shared_ptr<InputHandle> input;

	ic::Window window;
};

