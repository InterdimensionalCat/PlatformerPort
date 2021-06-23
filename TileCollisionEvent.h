#pragma once
#include "Tile.h"

enum class CollisionFace {
	WallLeft,
	WallRight,
	Floor,
	Ceiling
};

struct TileCollisionEvent
{
	Tile collidedTile;
	CollisionFace face;
};

