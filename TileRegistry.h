#pragma once

class TileBase;

void RegisterTiles();
std::shared_ptr<TileBase> getTileFromID(const sf::Color id);
std::shared_ptr<TileBase> getTileFromID(const uint32_t id);
std::shared_ptr<TileBase> getTileFromID(const uint8_t red,
	const uint8_t green,
	const uint8_t blue,
	const uint8_t alpha);

