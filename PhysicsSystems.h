#pragma once
#include "System.h"
#include "Tilemap.h"
#include "TileCollisionEvent.h"
#include "MapBoundCollisionEvent.h"

class UpdatePositions : public SystemTrivial<Transform, Velocity> {
public:
	UpdatePositions(Scene* scene) : SystemTrivial(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto trans = scene->getComponent<Transform>(entry);
		auto vel = scene->getComponent<Velocity>(entry);

		trans->x += vel->x;
		trans->y += vel->y;
	}
private:
	_System(UpdatePositions, SystemType::PostPhysics);
};

class TilemapCollision : public SystemTrivial<Transform, Velocity, Hitbox, TileCollidable> {
public:
	TilemapCollision(Scene* scene) : SystemTrivial<Transform, Velocity, Hitbox, TileCollidable>(scene), tilemap(scene->tilemap) {}
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
		hitbox = getPosSpeedAdjustedHitbox(entry);
		floorCollision(trans, vel, tiles, hitbox, originalhitbox, entry);
		tiles = tilemap->getTilesWithinArea(hitbox);
		hitbox = getPosSpeedAdjustedHitbox(entry);
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
						listener->events->push_back(TileCollisionEvent{ tile, CollisionFace::WallLeft });
					}

					// -> |
					trans->x = tile.getPosX() - originalhitbox->rect.width;
				} else {
					if (!tile.isRightActive()) {
						continue;
					}

					auto listener = scene->getComponent<TileCollisionEventListener>(entry);
					if (listener != nullptr) {
						listener->events->push_back(TileCollisionEvent{ tile, CollisionFace::WallRight });
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
					listener->events->push_back(TileCollisionEvent{ tile, CollisionFace::Floor });
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
					listener->events->push_back(TileCollisionEvent{ tile, CollisionFace::Ceiling });
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

	_System(TilemapCollision, SystemType::Physics);
};

class ApplyGravity : public SystemTrivial<PhysicsProperties, Velocity> {
public:
	ApplyGravity(Scene* scene) : SystemTrivial(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto grv = scene->getComponent<PhysicsProperties>(entry)->gravity;
		auto vel = scene->getComponent<Velocity>(entry);
		vel->y += grv;
	}

	_System(ApplyGravity, SystemType::PrePhysics);
};

class ApplyFriction : public SystemTrivial<PhysicsProperties, Velocity> {
public:
	ApplyFriction(Scene* scene) : SystemTrivial(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {

		auto input = scene->getComponent<InputController>(entry);

		auto frc = scene->getComponent<PhysicsProperties>(entry)->friction;
		auto vel = scene->getComponent<Velocity>(entry);

		if (input != nullptr) {
			auto right = input->input->isDown(InputButton::RIGHT);
			auto left =  input->input->isDown(InputButton::LEFT);
			if (right || left && !(right && left)) {
				frc = 1.0f;
			}
		}


		vel->x *= frc;
		if (abs(vel->x) < toMeters(0.2f)) {
			vel->x = 0;
		}
	}

	_System(ApplyFriction, SystemType::PrePhysics);
};

class BindMovement : public SystemTrivial<BoundedVelocity, Velocity> {
public:
	BindMovement(Scene* scene) : SystemTrivial(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto mvmt = scene->getComponent<BoundedVelocity>(entry);
		auto vel = scene->getComponent<Velocity>(entry);

		if (vel->x > mvmt->maxVelX) {
			vel->x = mvmt->maxVelX;
		}

		if (vel->x < -mvmt->maxVelX) {
			vel->x = -mvmt->maxVelX;
		}

		if (vel->y > mvmt->maxVelY) {
			vel->y = mvmt->maxVelY;
		}

		if (vel->y < -mvmt->maxVelY) {
			vel->y = -mvmt->maxVelY;
		}

	}

	_System(BindMovement, SystemType::Physics);
};

class BindPositionByMapBounds : public SystemTrivial<Transform, BoundByMapBounds> {
public:
	BindPositionByMapBounds(Scene* scene) : SystemTrivial(scene) {}
	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto transform = scene->getComponent<Transform>(entry);
		auto mapBounds = scene->getComponent<BoundByMapBounds>(entry);
		auto eventlistener = scene->getComponent<MapBoundCollisionListener>(entry);
		auto vel = scene->getComponent<Velocity>(entry);

		if (mapBounds->boundUp) {
			transform->y = std::max(transform->y, scene->getUpperBound());
			if (transform->y == scene->getUpperBound() && eventlistener != nullptr) {
				eventlistener->events->push_back(MapBoundCollisionEvent::HitUp);
				if (vel != nullptr) {
					vel->y = 0.0f;
				}
			}
		}

		if (mapBounds->boundDown) {
			transform->y = std::min(transform->y, scene->getLowerBound());
			if (transform->y == scene->getLowerBound() && eventlistener != nullptr) {
				eventlistener->events->push_back(MapBoundCollisionEvent::HitDown);
				if (vel != nullptr) {
					vel->y = 0.0f;
				}
			}
		}

		if (mapBounds->boundLeft) {
			transform->x = std::max(transform->x, scene->getLeftBound());
			if (transform->x == scene->getLeftBound() && eventlistener != nullptr) {
				eventlistener->events->push_back(MapBoundCollisionEvent::HitLeft);
				if (vel != nullptr) {
					vel->x = 0.0f;
				}
			}
		}

		if (mapBounds->boundRight) {
			transform->x = std::min(transform->x, scene->getRightBound());
			if (transform->x == scene->getRightBound() && eventlistener != nullptr) {
				eventlistener->events->push_back(MapBoundCollisionEvent::HitRight);
				if (vel != nullptr) {
					vel->x = 0.0f;
				}
			}
		}
	}

	_System(BindPositionByMapBounds, SystemType::PostUpdate);
};

class ActorCollision : public System {
public:
	ActorCollision(Scene* scene) : System(scene) {}

	virtual void excecute() {
		//keep track of the actors in a seperate vector in this system
		//use sweep and prune to reduce calculations

		collidableactors.clear();

		for (auto entry : SceneView<ActorCollidable, Transform, Hitbox>(scene)) {
			collidableactors.push_back(entry);
		}

		for (size_t i = 0; i < collidableactors.size(); i++) {

			for (size_t j = i + 1; j < collidableactors.size(); j++) {
				auto actorA = collidableactors.at(i);
				auto actorB = collidableactors.at(j);

				auto actorCollA = scene->getComponent<ActorCollidable>(actorA);
				auto actorCollB = scene->getComponent<ActorCollidable>(actorB);

				auto hitboxA = getPosSpeedAdjustedHitbox(scene, actorA);
				auto hitboxB = getPosSpeedAdjustedHitbox(scene, actorB);


				if (hitboxA.intersects(hitboxB)) {

					auto ABpair = std::pair(actorCollA->collisiontype, actorCollB->collisiontype);
					auto BApair = std::pair(actorCollB->collisiontype, actorCollA->collisiontype);

					auto foundcollfn = collisioncallbackmap.find(ABpair);
					if (foundcollfn != collisioncallbackmap.end()) {
						collisioncallbackmap.at(ABpair)(scene, actorA, actorB);
					}
					else {
						foundcollfn = collisioncallbackmap.find(BApair);
						if (foundcollfn != collisioncallbackmap.end()) {
							collisioncallbackmap.at(BApair)(scene, actorB, actorA);
						}
					}
				}
			}
		}
	}

	std::string toString() const override {
		std::string printval = getName()
			+ ":(System)->Type="
			+ getSysTypeName();
		return printval;
	}


private:

#ifndef addCollisionFn
#define addCollisionFn(actora, actorb) collisioncallbackmap.emplace(std::pair(#actora, #actorb), actora##to##actorb);
#endif

	class PopulateCallback {
	public:
		PopulateCallback() {
			addCollisionFn(player, enemy);
			addCollisionFn(player, platform);
			addCollisionFn(enemy, platform);
			addCollisionFn(player, goal);
			//collisioncallbackmap.emplace(std::pair("player", "enemy"), playertoenemy);
			//collisioncallbackmap.emplace(std::pair("player", "enemy"), playerToEnemy);
		}
	};

#undef addCollisionFn

	static inline std::map<std::pair<std::string, std::string>, std::function<void(Scene* scene, std::shared_ptr<ActorEntry>, std::shared_ptr<ActorEntry>)>> collisioncallbackmap;

	static inline PopulateCallback popcallback;

	static void playertogoal(Scene* scene, std::shared_ptr<ActorEntry> player, std::shared_ptr<ActorEntry> goal) {
		scene->pushEvent<ChangeLevelEvent>();
	}

	static void playertoenemy(Scene* scene, std::shared_ptr<ActorEntry> player, std::shared_ptr<ActorEntry> enemy) {


		auto ptrans = scene->getComponent<Transform>(player);
		auto pvel = scene->getComponent<Velocity>(player);
		auto phitbox = getPosSpeedAdjustedHitbox(scene, player);
		auto jumpdata = scene->getComponent<Jumpable>(player);

		auto etrans = scene->getComponent<Transform>(enemy);
		auto ehitbox = getPosSpeedAdjustedHitbox(scene, enemy);
		auto jumpondata = scene->getComponent<JumpOn>(enemy);

		auto playerYHigh = ptrans->y;
		auto enemyYHigh =  etrans->y;

		if (enemyYHigh - playerYHigh > ehitbox.height * jumpondata->jumpTolerencePercent) {
			//kill enemy

			jumpdata->jumpedflag = false;

			auto playervely = pvel->y;
			if (playervely > 0) {
				playervely *= -1.0f;
			}
			auto jumpforce = std::min(playervely * jumpondata->jumpVelMod, -jumpondata->jumpMin);
			pvel->y = jumpforce;

			scene->pushEvent<DespawnActorEvent>(enemy);
		}
		else {
			//killplayer
			playerDeath(scene, player);
		}

	}

	static void playertoplatform(Scene* scene, std::shared_ptr<ActorEntry> player, std::shared_ptr<ActorEntry> platform) {
  		creatureToPlatform(scene, player, platform);
	}

	static void enemytoplatform(Scene* scene, std::shared_ptr<ActorEntry> enemy, std::shared_ptr<ActorEntry> platform) {
		creatureToPlatform(scene, enemy, platform);
	}

	static void creatureToPlatform(Scene* scene, std::shared_ptr<ActorEntry> creature, std::shared_ptr<ActorEntry> platform) {

 		auto cstate = scene->getComponent<StateController>(creature)->state;
		auto cvel = scene->getComponent<Velocity>(creature);
		auto chitbox = getPosSpeedAdjustedHitbox(scene, creature);
		auto phitbox = getPosSpeedAdjustedHitboxNoExtension(scene, platform);

		auto cBottomCorner = chitbox.top + chitbox.height;
		auto pTopCorner = phitbox.top;

		if (cstate == ActionState::Airborne) {
			auto caabb = scene->getComponent<Hitbox>(creature)->rect;
			auto trans = scene->getComponent<Transform>(creature);

			caabb.left += trans->x;
			caabb.top  += trans->y;
			auto cbighitbox = getBoundingBoxOfBoxes(caabb, chitbox);
			//auto cbighitbox = chitbox;
			if (!cbighitbox.intersects(phitbox)) {
				return;
			}
		}
		else {
			if (chitbox.top > phitbox.top) return;
		}

  		auto ptolerence = scene->getComponent<PlatformTolerence>(platform);
		if (cBottomCorner - ptolerence->down < pTopCorner && cvel->y >= 0) {
			auto ctrans = scene->getComponent<Transform>(creature);
			auto pvel = scene->getComponent<Velocity>(platform);
			auto ptype = scene->getComponent<PlatformTypeable>(platform)->type;
			ctrans->y = phitbox.top - chitbox.height;
			
			auto velsignbit = pvel->y < 0 ? -1 : 1;

			auto grav = scene->getComponent<PhysicsProperties>(creature)->gravity;

			switch (ptype) {
			case PlatformType::Horizontal:
				if (cstate == ActionState::Airborne) {
					cvel->x = 0.0f;
				}

				cvel->y = 0.0f;
				ctrans->x += pvel->x;
				break;
			case PlatformType::Vertical:

				if (pvel->y > 0) {
					ctrans->y -= pvel->y;
					cvel->y = pvel->y - grav;
				}
				else {
					cvel->y = pvel->y + ptolerence->up;
				}
				break;
			case PlatformType::Falling:
				cvel->y = pvel->y - grav;
				scene->getComponent<FallingPlatform>(platform)->falling = true;
				ctrans->y -= pvel->y;
				cvel->y = pvel->y - grav;
				break;
			default:
				cvel->y = 0.0f;
				break;
			}



			auto airbornetimer = scene->getComponent<AirborneState>(creature);
			airbornetimer->airborneTimer = airbornetimer->maxAirborneTimer;
			if (abs(cvel->x) < toMeters(0.5f)) {
				scene->getComponent<StateController>(creature)->state = ActionState::GroundStill;
			}
			else {
				scene->getComponent<StateController>(creature)->state = ActionState::GroundMove;
			}

			//TODO: FINISH THIS NIGHTMERE XD

			//need to make an actor collision event class and relevant components
		}

	}

	static sf::FloatRect getBoundingBoxOfBoxes(const sf::FloatRect& A, const sf::FloatRect& B) {
		auto A1 = sf::Vector2f(A.left, A.top);
		auto A2 = sf::Vector2f(A.left + A.width, A.top);
		auto A3 = sf::Vector2f(A.left + A.width, A.top + A.height);
		auto A4 = sf::Vector2f(A.left, A.top + A.height);

		auto B1 = sf::Vector2f(B.left, B.top);
		auto B2 = sf::Vector2f(B.left + B.width, B.top);
		auto B3 = sf::Vector2f(B.left + B.width, B.top + B.height);
		auto B4 = sf::Vector2f(B.left, B.top + B.height);

		auto C1 = sf::Vector2f(std::min(A1.x, B1.x), std::min(A1.y, B1.y));
		auto C2 = sf::Vector2f(std::min(A2.x, B2.x), std::max(A2.y, B2.y));
		auto C3 = sf::Vector2f(std::max(A3.x, B3.x), std::max(A3.y, B3.y));
		auto C4 = sf::Vector2f(std::max(A4.x, B4.x), std::min(A4.y, B4.y));

		auto crect = sf::FloatRect(C1, C3 - C1);

		return crect;
	}

	static void playerDeath(Scene* scene, std::shared_ptr<ActorEntry> player) {
		scene->pushEvent<ResetLevelEvent>();
	}

	static sf::FloatRect getPosSpeedAdjustedHitboxNoExtension(Scene* scene, std::shared_ptr<ActorEntry> entry) {
		auto aabbcomp = scene->getComponent<Hitbox>(entry);
		auto trans = scene->getComponent<Transform>(entry);
		auto vel = scene->getComponent<Velocity>(entry);

		auto aabb = aabbcomp->rect;

		auto veldispx = 0.0f;
		auto veldispy = 0.0f;

		if (vel != nullptr) {
			veldispx = vel->x;
			veldispy = vel->y;
		}

		aabb.left += trans->x + veldispx;
		aabb.top += trans->y + veldispy;

		drawHitbox(scene, aabb);

		return aabb;
	}

	static sf::FloatRect getPosSpeedAdjustedHitbox(Scene* scene, std::shared_ptr<ActorEntry> entry) {
		auto aabbcomp = scene->getComponent<Hitbox>(entry);
		auto trans = scene->getComponent<Transform>(entry);
		auto vel = scene->getComponent<Velocity>(entry);

		sf::FloatRect aabb = aabbcomp->rect;
		auto displacementvec = sf::Vector2f(0, 0);
		auto aabbcompbig = scene->getComponent<HitboxExtension>(entry);
		if (aabbcompbig != nullptr) {
			aabb = aabbcompbig->rect;

			displacementvec = sf::Vector2f(
				aabb.left + aabb.width / 2.0f - aabbcomp->rect.left - aabbcomp->rect.width / 2.0f,
				aabb.top + aabb.height / 2.0f - aabbcomp->rect.top - aabbcomp->rect.height / 2.0f);
		}

		auto veldispx = 0.0f;
		auto veldispy = 0.0f;

		if (vel != nullptr) {
			veldispx = vel->x;
			veldispy = vel->y;
		}


		aabb.left += trans->x + veldispx - displacementvec.x;
		aabb.top += trans->y  + veldispy - displacementvec.y;

		drawHitbox(scene, aabb);

		return aabb;
	}

	static void drawHitbox(Scene* scene, const sf::FloatRect& rect) {
		sf::RectangleShape shape;
		shape.setPosition(toPixels(rect.left), toPixels(rect.top));
		shape.setSize(sf::Vector2f(toPixels(rect.width), toPixels(rect.height)));
		shape.setOutlineThickness(3.0f);
		shape.setFillColor(sf::Color(0, 0, 0, 0));
		shape.setOutlineColor(sf::Color::Red);

		scene->debugShapes.push_back(shape);
	}

	std::vector<std::shared_ptr<ActorEntry>> collidableactors;

	_System(ActorCollision, SystemType::Physics);
};