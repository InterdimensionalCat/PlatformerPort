#include "stdafx.h"
#include "TileRegistry.h"
#include "Tile.h"

static std::map<uint32_t, std::shared_ptr<Tile>> idMap = std::map<uint32_t, std::shared_ptr<Tile>>();

const Tile& getTileFromID(const sf::Color id) {
	return getTileFromID(id.toInteger());
}

const Tile& getTileFromID(const uint32_t id) {
	auto found = idMap.find(id);
	if (found != idMap.end()) {
		return *found->second;
	}
	else {
		return *idMap.at(0x54AAF5FF);
	}
}

const Tile& getTileFromID(const uint8_t red,
	const uint8_t green,
	const uint8_t blue,
	const uint8_t alpha) {
	return getTileFromID(sf::Color(red, green, blue, alpha).toInteger());
}

void RegisterTile(std::shared_ptr<Tile> tile) {
	idMap.emplace(tile->getID().toInteger(), tile);
	Logger::get() << "Tile registered with id: " << (uint32_t)(tile->getID().toInteger()) << "\n";
}

void RegisterTiles() {
	RegisterTile(std::make_shared<Tile>(0x54AAF5FF, TileType::Air,   1, 1));

	RegisterTile(std::make_shared<Tile>(0x000000FF, TileType::Solid, 3, 1));
	RegisterTile(std::make_shared<Tile>(0xFF0000FF, TileType::Solid, 2, 1));
	RegisterTile(std::make_shared<Tile>(0xFFFFFFFF, TileType::Solid, 4, 1));
	RegisterTile(std::make_shared<Tile>(0x00FFFFFF, TileType::Solid, 1, 2));
	RegisterTile(std::make_shared<Tile>(0x00FF00FF, TileType::Solid, 2, 2));
	RegisterTile(std::make_shared<Tile>(0xF0F0F0FF, TileType::Solid, 3, 2));
	RegisterTile(std::make_shared<Tile>(0x0F0F0FFF, TileType::Solid, 4, 2));
	RegisterTile(std::make_shared<Tile>(0x111111FF, TileType::Solid, 1, 3));
	RegisterTile(std::make_shared<Tile>(0x222222FF, TileType::Solid, 2, 3));
}