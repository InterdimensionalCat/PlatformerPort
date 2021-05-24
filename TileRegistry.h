#pragma once

class Tile;

void RegisterTiles();
const Tile& getTileFromID(const sf::Color id);
const Tile& getTileFromID(const uint32_t id);
const Tile& getTileFromID(const uint8_t red,
	const uint8_t green,
	const uint8_t blue,
	const uint8_t alpha);

