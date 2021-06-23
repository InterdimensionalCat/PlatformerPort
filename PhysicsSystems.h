#pragma once
#include "System.h"
#include "Tilemap.h"
#include "TileCollisionEvent.h"

class UpdatePositions : public System<Transform, Velocity> {
public:
	UpdatePositions(Scene* scene) : System(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto trans = scene->getComponent<Transform>(entry);
		auto vel = scene->getComponent<Velocity>(entry);

		trans->x += vel->x;
		trans->y += vel->y;
	}
};

class TilemapCollision : public System<Transform, Velocity, Hitbox, TileCollidable> {
public:
	TilemapCollision(Scene* scene, std::shared_ptr<Tilemap> tilemap) : System<Transform, Velocity, Hitbox, TileCollidable>(scene), tilemap(tilemap) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {

		if (!scene->getComponent<TileCollidable>(entry)->tileCollidable) {
			return;
		}

		auto trans = scene->getComponent<Transform>(entry);
		auto vel = scene->getComponent<Velocity>(entry);
		auto originalhitbox = scene->getComponent<Hitbox>(entry);

		auto hitbox = getPosSpeedAdjustedHitbox(entry);
		auto tiles = tilemap->getTilesWithinArea(hitbox);
		wallCollision(trans, vel, tiles, hitbox, originalhitbox, entry);
		tiles = tilemap->getTilesWithinArea(hitbox);
		floorCollision(trans, vel, tiles, hitbox, originalhitbox, entry);
		tiles = tilemap->getTilesWithinArea(hitbox);
		ceilingCollision(trans, vel, tiles, hitbox, originalhitbox, entry);

	}
private:

	void wallCollision(Transform* trans, Velocity* vel, 
		const std::vector<Tile>& tiles, const sf::FloatRect& hitbox, 
		const Hitbox* originalhitbox, std::shared_ptr<ActorEntry> entry) {
		if (vel->x == 0) {
			return;
		}

		for (auto& tile : tiles) {

			if (hitbox.top + hitbox.height * 0.2f < tile.getPosY() && tile.isTopActive()) continue;
			if (hitbox.top + hitbox.height * 0.8f > tile.getPosY() + 1.0f && tile.isBotActive()) continue;

			if (hitbox.intersects(sf::FloatRect(tile.getPosX(), tile.getPosY(), 1.0f, 1.0f))) {
				if (vel->x > 0) {
					if (!tile.isLeftActive()) {
						continue;
					}

					auto listener = scene->getComponent<TileCollisionEventListener>(entry);
					if (listener != nullptr) {
						listener->events.push(TileCollisionEvent{ tile, CollisionFace::WallLeft });
					}

					// -> |
					trans->x = tile.getPosX() - originalhitbox->rect.width;
				} else {
					if (!tile.isRightActive()) {
						continue;
					}

					auto listener = scene->getComponent<TileCollisionEventListener>(entry);
					if (listener != nullptr) {
						listener->events.push(TileCollisionEvent{ tile, CollisionFace::WallRight });
					}

					// | <-
					trans->x = tile.getPosX() + 1.0f;
				}
				vel->x = 0.0f;
				return;
			}
		}
	}

	void floorCollision(Transform* trans, Velocity* vel, 
		const std::vector<Tile>& tiles, const sf::FloatRect& hitbox, 
		const Hitbox* originalhitbox, std::shared_ptr<ActorEntry> entry) {

		if (vel->y <= 0) {
			return;
		}

		for (auto& tile : tiles) {
			if (hitbox.top > tile.getPosY() + 0.2f) continue;
			if (hitbox.top + hitbox.height > tile.getPosY() + 0.8f) continue;

			if (hitbox.intersects(sf::FloatRect(tile.getPosX(), tile.getPosY(), 1.0f, 1.0f))) {
				if (!tile.isTopActive()) {
					continue;
				}

				auto listener = scene->getComponent<TileCollisionEventListener>(entry);
				if (listener != nullptr) {
					listener->events.push(TileCollisionEvent{ tile, CollisionFace::Floor });
				}

				trans->y = tile.getPosY() - originalhitbox->rect.height;
				vel->y = 0.0f;
			}
		}
	}

	void ceilingCollision(Transform* trans, Velocity* vel,
		const std::vector<Tile>& tiles, const sf::FloatRect& hitbox,
		const Hitbox* originalhitbox, std::shared_ptr<ActorEntry> entry) {

		if (vel->y >= 0) {
			return;
		}

		for (auto& tile : tiles) {

			if (hitbox.top < tile.getPosY() + 0.2f) continue;
			if (hitbox.top + hitbox.height < tile.getPosY() + 0.8f) continue;

			if (hitbox.intersects(sf::FloatRect(tile.getPosX(), tile.getPosY(), 1.0f, 1.0f))) {

				if (!tile.isBotActive()) {
					continue;
				}

				auto listener = scene->getComponent<TileCollisionEventListener>(entry);
				if (listener != nullptr) {
					listener->events.push(TileCollisionEvent{ tile, CollisionFace::Ceiling });
				}

				trans->y = tile.getPosY() + 1.0f;
				vel->y = 0.0f;
				return;
			}
		}
	}

	sf::FloatRect getPosSpeedAdjustedHitbox(std::shared_ptr<ActorEntry> entry) {
		auto aabbcomp = scene->getComponent<Hitbox>(entry);
		auto trans = scene->getComponent<Transform>(entry);
		auto vel = scene->getComponent<Velocity>(entry);

		auto aabb = aabbcomp->rect;

		aabb.left += trans->x + vel->x;
		aabb.top  += trans->y + vel->y;

		return aabb;
	}

	std::shared_ptr<Tilemap> tilemap;
};

class ApplyGravity : public System<PhysicsProperties, Velocity> {
public:
	ApplyGravity(Scene* scene) : System(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto grv = scene->getComponent<PhysicsProperties>(entry)->gravity;
		auto vel = scene->getComponent<Velocity>(entry);
		vel->y += grv;
	}
};
