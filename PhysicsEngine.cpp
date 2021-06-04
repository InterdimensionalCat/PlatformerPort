#include "stdafx.h"
#include "PhysicsEngine.h"
#include "Actor.h"
#include "Tilemap.h"
#include "Tile.h"
#include "LevelData.h"

PhysicsEngine::PhysicsEngine(const LevelData& data) : scene(data.scene) {

}

void PhysicsEngine::update(Bodies& bodies, Tilemap& tilemap) {



	for (auto& body : bodies) {

		if (!body->collidable()) {
			continue;
		}

		auto hitbox = getSpeedAdjustedAABB(body);
		auto tiles = tilemap.getTilesWithinArea(hitbox);
		wallCollision(body, tiles, hitbox);
		hitbox = getSpeedAdjustedAABB(body);
		floorCollision(body, tiles, hitbox);
		hitbox = getSpeedAdjustedAABB(body);
		ceilingCollision(body, tiles, hitbox);
	}

	entityCollision(bodies);

	checkMapBounds(bodies, tilemap);

	updatePositions(bodies);
}

void PhysicsEngine::checkMapBounds(Bodies& bodies, Tilemap& tilemap) {
	for (auto& body : bodies) {
		auto hitbox = getSpeedAdjustedAABB(body);
		float width = tilemap.getWidth();
		float height = tilemap.getHeight();

		//top

		if (hitbox.top < 0.0f && body->getVelY() < 0) {
			//body->setPosY(0.0f);
			//body->setVelY(0.0f);
		}

		//left

		if (hitbox.left < 0.0f && body->getVelX() < 0) {
			body->setPosX(0.0f);
			body->setVelX(0.0f);
		}

		//bot

		//if (hitbox.top + hitbox.height > height && body->getVelY() > 0) {
		//	body->setPosY(height - hitbox.height);
		//	body->setVelY(0.0f);
		//}

		if (hitbox.top > height) {
			//body->setPosY(height - hitbox.height);
			//body->setVelY(0.0f);
			body->despawn();
		}

		//right

		if (hitbox.left + hitbox.width > width && body->getVelX() > 0) {
			body->setPosX(width - hitbox.width);
			body->setVelX(0.0f);
		}

	}
}

sf::FloatRect PhysicsEngine::getSpeedAdjustedAABB(Body& body) {
	auto rect = body->getPosAdjustedAABB();
	return sf::FloatRect(rect.left + body->getVelX(), rect.top + body->getVelY(), rect.width, rect.height);
}

void PhysicsEngine::updatePositions(Bodies& bodies) {
	for (auto& body : bodies) {
		body->setPosX(body->getPosX() + body->getVelX());
		body->setPosY(body->getPosY() + body->getVelY());
	}
}

void PhysicsEngine::wallCollision(Body& body, const std::vector<Tile>& tiles, const sf::FloatRect& hitbox) {
	if (body->getVelX() == 0) {
		return;
	}

	for (auto& tile : tiles) {

		if (hitbox.top + hitbox.height*0.2f < tile.getPosY() && tile.isTopActive()) continue;
		if (hitbox.top + hitbox.height*0.8f > tile.getPosY() + 1.0f && tile.isBotActive()) continue;

		if (hitbox.intersects(sf::FloatRect(tile.getPosX(), tile.getPosY(), 1.0f, 1.0f))) {
			if (body->getVelX() > 0) {
				if (!tile.isLeftActive()) {
					continue;
				}

				body->onTileCollision(tile, CollisionType::Wall);

				// -> |
				body->setPosX(tile.getPosX() - body->getPosAdjustedAABB().width);
			}
			else {
				if (!tile.isRightActive()) {
					continue;
				}

				body->onTileCollision(tile, CollisionType::Wall);

				// | <-
				body->setPosX(tile.getPosX() + 1.0f);
			}

			body->setVelX(0.0f);
			return;
		}
	}
}
void PhysicsEngine::ceilingCollision(Body& body, const std::vector<Tile>& tiles, const sf::FloatRect& hitbox) {
	if (body->getVelY() >= 0) {
		return;
	}

	for (auto& tile : tiles) {

		if (hitbox.top < tile.getPosY() + 0.2f) continue;
		if (hitbox.top + hitbox.height < tile.getPosY() + 0.8f) continue;

		if (hitbox.intersects(sf::FloatRect(tile.getPosX(), tile.getPosY(), 1.0f, 1.0f))) {

			if (!tile.isBotActive()) {
				continue;
			}

			body->onTileCollision(tile, CollisionType::Ceiling);

			body->setPosY(tile.getPosY() + 1.0f);
			body->setVelY(0.0f);
			return;
		}
	}
}
void PhysicsEngine::floorCollision(Body& body, const std::vector<Tile>& tiles, const sf::FloatRect& hitbox) {
	if (body->getVelY() <= 0) {
		return;
	}

	for (auto& tile : tiles) {

		if (hitbox.top > tile.getPosY() + 0.2f) continue;
		if (hitbox.top + hitbox.height > tile.getPosY() + 0.8f) continue;

		if (hitbox.intersects(sf::FloatRect(tile.getPosX(), tile.getPosY(), 1.0f, 1.0f))) {

			if (!tile.isTopActive()) {
				continue;
			}

			body->onTileCollision(tile, CollisionType::Floor);

			body->setPosY(tile.getPosY() - body->getPosAdjustedAABB().height);
			body->setVelY(0.0f);
		}
	}
}

void PhysicsEngine::entityCollision(Bodies& bodies) {
	for (size_t i = 0; i < bodies.size(); i++) {
		for (size_t j = i + 1; j < bodies.size(); j++) {
			auto& bodyA = bodies.at(i);
			auto& bodyB = bodies.at(j);

			auto hitboxA = getSpeedAdjustedAABB(bodyA);
			auto hitboxB = getSpeedAdjustedAABB(bodyB);

			hitboxA.top -= bodyA->collisionTolerenceY;
			hitboxA.height += bodyA->collisionTolerenceY;

			hitboxB.top -= bodyB->collisionTolerenceY;
			hitboxB.height += bodyB->collisionTolerenceY;

			if (hitboxA.intersects(hitboxB)) {
				bodyA->onCollision(bodyB);
			    bodyB->onCollision(bodyA);
			}
		}
	}
}