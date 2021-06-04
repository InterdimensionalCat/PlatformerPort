#include "stdafx.h"
#include "TileRegistry.h"
#include "Tile.h"

static std::map<uint32_t, std::shared_ptr<TileBase>> idMap = std::map<uint32_t, std::shared_ptr<TileBase>>();

std::shared_ptr<TileBase> getTileFromID(const sf::Color id) {
	return getTileFromID(id.toInteger());
}

std::shared_ptr<TileBase> getTileFromID(const uint32_t id) {
	auto found = idMap.find(id);
	if (found != idMap.end()) {
		return found->second;
	}
	else {
		return idMap.at(0x54AAF5FF);
	}
}

std::shared_ptr<TileBase> getTileFromID(const uint8_t red,
	const uint8_t green,
	const uint8_t blue,
	const uint8_t alpha) {
	return getTileFromID(sf::Color(red, green, blue, alpha).toInteger());
}

void RegisterTile(std::shared_ptr<TileBase> tile) {
	idMap.emplace(tile->getID().toInteger(), tile);
	Logger::get() << "Tile registered with id: " << (uint32_t)(tile->getID().toInteger()) << "\n";
}

void RegisterTiles() {
	RegisterTile(std::make_shared<TileBase>(0x54AAF5FF, TileType::Air,   1, 1));

	RegisterTile(std::make_shared<TileBase>(0x000000FF, TileType::Solid, 3, 1));
	RegisterTile(std::make_shared<TileBase>(0xFF0000FF, TileType::Solid, 2, 1));
	RegisterTile(std::make_shared<TileBase>(0xFFFFFFFF, TileType::Solid, 4, 1));
	RegisterTile(std::make_shared<TileBase>(0x00FFFFFF, TileType::Solid, 1, 2));
	RegisterTile(std::make_shared<TileBase>(0x00FF00FF, TileType::Solid, 2, 2));
	RegisterTile(std::make_shared<TileBase>(0xF0F0F0FF, TileType::Solid, 3, 2));
	RegisterTile(std::make_shared<TileBase>(0x0F0F0FFF, TileType::Solid, 4, 2));
	RegisterTile(std::make_shared<TileBase>(0x111111FF, TileType::Solid, 1, 3));
	RegisterTile(std::make_shared<TileBase>(0x222222FF, TileType::Solid, 2, 3));
}