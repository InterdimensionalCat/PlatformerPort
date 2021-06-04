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

	std::vector<Tile> getTilesWithinArea(const sf::FloatRect& hitbox);

private:
	void setTile(uint32_t x, uint32_t y, const std::shared_ptr<TileBase> tile);

	void disableInactiveSides();

	//x, y order
	std::vector<std::vector<Tile>> tiles;

	std::unique_ptr<ic::Texture> tileset;

	std::unique_ptr<sf::RenderTexture> rendertex;

	sf::VertexArray vertices;

	LevelData currentMap;
	float width = 0;
	float height = 0;
};

