#pragma once
#include "Tile.h"
#include "LevelData.h"
#include "Window.h"
#include "Texture.h"

class Tilemap
{
public:
	Tilemap(const LevelData& firstMap);
	void loadMap(const LevelData& map);
	float getWidth() const;
	float getHeight() const;

	void draw(ic::Window& window) const;

private:
	void setTile(uint32_t x, uint32_t y, const Tile& tile);

	//x, y order
	std::vector<std::vector<Tile>> tiles;

	std::unique_ptr<ic::Texture> tileset;

	sf::VertexArray vertices;

	LevelData currentMap;
	float width = 0;
	float height = 0;
};

