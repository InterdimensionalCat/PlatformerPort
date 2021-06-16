#include "stdafx.h"
#include "Tile.h"

TileBase::TileBase() : TileBase(0, TileType::Air, 1, 1) {}

TileBase::TileBase(const uint32_t id,
	const TileType type,
	const uint32_t tilesetX,
	const uint32_t tilesetY) : id(id), type(type), tilesetX(tilesetX), tilesetY(tilesetY) {
}

const uint32_t TileBase::getID() const {
	return id;
}

const TileType& TileBase::getType() const {
	return type;
}

const uint32_t TileBase::getTilesetX() const {
	return tilesetX;
}

const uint32_t TileBase::getTilesetY() const {
	return tilesetY;
}

Tile::Tile() : base(std::make_shared<TileBase>()), pos(sf::Vector2f(0, 0)), activeSides({ true, true, true, true }) {}

Tile::Tile(std::shared_ptr<TileBase> base, const float posX, const float posY) :
	base(base),
	pos(sf::Vector2f(posX, posY)),
	activeSides({ true, true, true, true }) {}

bool Tile::isLeftActive() const {
	return activeSides.at(3);
}

bool Tile::isRightActive() const {
	return activeSides.at(1);
}

bool Tile::isTopActive() const {
	return activeSides.at(0);
}

bool Tile::isBotActive() const {
	return activeSides.at(2);
}

const uint32_t Tile::getID() const {
	return base->id;
}

const TileType& Tile::getType() const {
	return base->type;
}

uint32_t Tile::getTilesetX() const {
	return base->tilesetX;
}

uint32_t Tile::getTilesetY() const {
	return base->tilesetY;
}

float Tile::getPosX() const {
	return pos.x;
}

float Tile::getPosY() const {
	return pos.y;
}
