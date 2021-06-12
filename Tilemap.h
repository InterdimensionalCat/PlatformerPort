#pragma once
#include "Tile.h"
#include "Texture.h"
#include "LevelData.h"

class Window;

class Tilemap
{
public:
	Tilemap(const LevelData& firstMap);
	void loadMap(const LevelData& map);
	float getWidth() const;
	float getHeight() const;

	void draw(Window& window) const;

	std::vector<Tile> getTilesWithinArea(const sf::FloatRect& hitbox);

private:
	void setTile(uint32_t x, uint32_t y, const std::shared_ptr<TileBase> tile);

	void disableInactiveSides();

	//x, y order
	std::vector<std::vector<Tile>> tiles;

	std::unique_ptr<Texture> tileset;

	std::unique_ptr<sf::RenderTexture> rendertex;

	sf::VertexArray vertices;

	LevelData currentMap;
	float width = 0;
	float height = 0;
};

