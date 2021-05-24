#pragma once

enum class TileType {Solid, Air};

class Tile
{
public:

	Tile() : Tile(0x54AAF500, TileType::Air, 1, 1) {}

	Tile(const uint32_t id, 
		const TileType type,
		const uint32_t tilesetX,
		const uint32_t tilesetY) : id(sf::Color(id)), type(type), tilesetX(tilesetX), tilesetY(tilesetY) {
	}

	const sf::Color& getID() const {
		return id;
	}

	const TileType& getType() const {
		return type;
	}

	const uint32_t getTilesetX() const {
		return tilesetX;
	}

	const uint32_t getTilesetY() const {
		return tilesetY;
	}

private:
	sf::Color id;
	TileType type;
	uint32_t tilesetX;
	uint32_t tilesetY;
};

