#include "stdafx.h"
#include "TileRegistry.h"
#include "Tile.h"

static std::map<uint32_t, std::shared_ptr<TileBase>> idMap = std::map<uint32_t, std::shared_ptr<TileBase>>();

std::shared_ptr<TileBase> getTileFromID(const uint32_t id) {
	auto found = idMap.find(id);
	if (found != idMap.end()) {
		return found->second;
	}
	else {
		return idMap.at(0);
	}
}

void RegisterTile(std::shared_ptr<TileBase> tile) {
	idMap.emplace(tile->getID(), tile);
	Logger::get() << "Tile registered with id: " << (uint32_t)(tile->getID()) << "\n";
}

void RegisterTiles() {
	int count = 1;
	for (int y = 1; y <= 4; y++) {
		for (int x = 1; x <= 4; x++) {
			if (count == 1) {
				RegisterTile(std::make_shared<TileBase>(count, TileType::Air, x, y));
			}
			else {
				RegisterTile(std::make_shared<TileBase>(count, TileType::Solid, x, y));
			}
			count++;
		}
	}
}