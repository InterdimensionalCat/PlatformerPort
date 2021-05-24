#pragma once
#include "Tilemap.h"
#include "LevelRegistry.h"
#include "TIleRegistry.h"
#include "Window.h"

class Scene
{
public:
	Scene() {
		init();
		tilemap = std::make_unique<Tilemap>(getLevelData(1));
	}
	void update() {
		window.updateInput();
	}
	void draw(const float interpol) {
		window.preRender(interpol);

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
	ic::Window window;
};

