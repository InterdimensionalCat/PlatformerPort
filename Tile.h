#pragma once
#include <array>

enum class TileType {Solid, Air};

class TileBase
{
public:

	TileBase() : TileBase(0x54AAF500, TileType::Air, 1, 1) {}

	TileBase(const uint32_t id, 
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

	friend class Tile;

	sf::Color id;
	TileType type;
	uint32_t tilesetX;
	uint32_t tilesetY;
};

class Tile {
public:

	Tile() : base(std::make_shared<TileBase>()), pos(sf::Vector2f(0, 0)) {}

	Tile(std::shared_ptr<TileBase> base, const float posX, const float posY) : 
		base(base), 
		pos(sf::Vector2f(posX, posY)), 
		activeSides({ true, true, true, true }) {}

	bool isLeftActive() const {
		return activeSides.at(3);
	}

	bool isRightActive() const {
		return activeSides.at(1);
	}

	bool isTopActive() const {
		return activeSides.at(0);
	}

	bool isBotActive() const {
		return activeSides.at(2);
	}

	const sf::Color& getID() const {
		return base->id;
	}

	const TileType& getType() const {
		return base->type;
	}

	uint32_t getTilesetX() const {
		return base->tilesetX;
	}

	uint32_t getTilesetY() const {
		return base->tilesetY;
	}

	float getPosX() const {
		return pos.x;
	}

	float getPosY() const {
		return pos.y;
	}

private:

	friend class Tilemap;

	std::shared_ptr<TileBase> base;
	sf::Vector2f pos;
	std::array<bool, 4> activeSides;
};

