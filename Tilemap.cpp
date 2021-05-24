#include "stdafx.h"
#include "Tilemap.h"
#include "Texture.h"
#include "Image.h"
#include "TileRegistry.h"
#include "Window.h"

Tilemap::Tilemap(const LevelData& firstMap) : currentMap(firstMap) {
	loadMap(firstMap);
}
void Tilemap::loadMap(const LevelData& map) {

	currentMap = map;
	ic::Image mapData(map.levelname);
	height = mapData.height();
	width = mapData.width();

	tiles.resize((size_t)width);
	for (auto& column : tiles) {
		column.resize((size_t)height);
	}

	for (uint32_t y = 0; y < height; y++) {
		for (uint32_t x = 0; x < width; x++) {
			auto id = mapData.getColorAtPixel(x, y);
			setTile(x, y, getTileFromID(id));
		}
	}

	tileset = std::make_unique<ic::Texture>(map.tilesetname);

#pragma warning( push )
#pragma warning( disable : 4244 )

    // resize the vertex array to fit the level size
    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(width * height * 4);

    // populate the vertex array, with one quad per tile

    for (unsigned int i = 0; i < width; ++i) {
        for (unsigned int j = 0; j < height; ++j)
        {
            sf::Vector2u tileSize = sf::Vector2u((unsigned int)toPixels(1), (unsigned int)toPixels(1));

            // get the current tile number
            auto tileAt = tiles.at(i).at(j);

            // find its position in the tileset texture
            int tu = tileAt.getTilesetX() - 1;
            int tv = tileAt.getTilesetY() - 1;

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &vertices[(i + j * width) * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

        }
    }

#pragma warning( pop )

}

void Tilemap::setTile(uint32_t x, uint32_t y, const Tile& tile) {
	tiles.at(x).at(y) = tile;
}

float Tilemap::getWidth() const {
	return width;
}

float Tilemap::getHeight() const {
	return height;
}

void Tilemap::draw(ic::Window& window) const
{
    // apply the transform
    //window.states.transform *= getTransform();

    // apply the tileset texture
    window.states.texture = &tileset->getTexture();

    // draw the vertex array
    window.window->draw(vertices, window.states);
}

