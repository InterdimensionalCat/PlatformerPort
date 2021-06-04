#pragma once

class Actor;
class Tiles;
class Tilemap;
class Tile;

class PhysicsEngine
{

	typedef std::vector<std::shared_ptr<Actor>> Bodies;
	typedef std::shared_ptr<Actor> Body;
public:
	void update(Bodies& bodies, Tilemap& tilemap);
	void updatePositions(Bodies& bodies);

	void checkMapBounds(Bodies& bodies, Tilemap& tilemap);

	void wallCollision(Body& body, const std::vector<Tile>& tiles, const sf::FloatRect& hitbox);
	void ceilingCollision(Body& body, const std::vector<Tile>& tiles, const sf::FloatRect& hitbox);
	void floorCollision(Body& body, const std::vector<Tile>& tiles, const sf::FloatRect& hitbox);

	void entityCollision(Bodies& bodies);

	sf::FloatRect getSpeedAdjustedAABB(Body& body);
};

