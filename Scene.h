#pragma once
#include "Tilemap.h"
#include "LevelRegistry.h"
#include "TIleRegistry.h"
#include "Window.h"
#include "Parallax.h"

class Scene
{
public:
	Scene() {
		init();
		auto& firstLevel = getLevelData(1);
		tilemap = std::make_unique<Tilemap>(firstLevel);
		parallaxEngine = std::make_unique<Parallax>(firstLevel);
	}
	void update() {
		window.updateInput();
		parallaxEngine->move();
	}
	void draw(const float interpol) {
		window.preRender(interpol);

		parallaxEngine->draw(window);
		tilemap->draw(window);

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
	//camera
	//background layers
	std::unique_ptr<Parallax> parallaxEngine;
	ic::Window window;
};

