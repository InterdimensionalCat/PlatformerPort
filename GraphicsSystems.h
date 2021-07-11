#pragma once
#include "System.h"
#include "SFML/Graphics.hpp"
#include "Animation.h"

class DrawSprites : public System<SpriteDrawable> {
public:
	DrawSprites(Scene* scene, std::shared_ptr<Window> window) : System(scene), window(window) {}

	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto sprite = scene->getComponent<SpriteDrawable>(entry);
		sprite->spr->setTexture(sprite->tex->getTexture());
		window->window->draw(*sprite->spr);
	}
private:
	std::shared_ptr<Window> window;
};

class UpdateSpritePos : public System<SpriteDrawable, Transform> {
public:
	UpdateSpritePos(Scene* scene) : System(scene) {}

	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto sprite = scene->getComponent<SpriteDrawable>(entry);
		auto trans = scene->getComponent<Transform>(entry);
		sf::Sprite& spr = *sprite->spr;
		spr.setPosition(toPixels(trans->x), toPixels(trans->y));
	}
};

class DrawAnimations : public System<Animatable> {
public:
	DrawAnimations(Scene* scene, std::shared_ptr<Window> window) : System(scene), window(window) {}

	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto anim = scene->getComponent<Animatable>(entry);
		anim->animation->draw(*window);
	}
private:
	std::shared_ptr<Window> window;
};

class StateAnimationDriver : public System<Animatable, Transform, StateController, Velocity, AnimStateMap, Hitbox, InputController> {
public:
	StateAnimationDriver(Scene* scene) : System(scene) {}

	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto anim = scene->getComponent<Animatable>(entry);
		auto trans = scene->getComponent<Transform>(entry);
		auto vel = scene->getComponent<Velocity>(entry);
		auto state = scene->getComponent<StateController>(entry);
		auto statemap = scene->getComponent<AnimStateMap>(entry);

		//change anim based on state
		if (statemap->prevstate != state->state) {
 			statemap->prevstate = state->state;
			bool prevflip = anim->animation->flip;
			anim->animation = std::make_shared<Animation>(statemap->animations->at(state->state), prevflip);
		}

		//determine if it needs to be flipped
		if (state->state == ActionState::GroundMove || state->state == ActionState::GroundTurn || state->state == ActionState::GroundStill) {

			if (state->state == ActionState::GroundStill) {
				auto input = scene->getComponent<InputController>(entry);
				if (input->input->isDown(InputButton::LEFT) && !anim->animation->flip) {
					anim->animation->setFlipped(true);
				}

				if (input->input->isDown(InputButton::RIGHT) && anim->animation->flip) {
					anim->animation->setFlipped(false);
				}
			}

			if (anim->animation->flip && vel->x > 0) {
				anim->animation->setFlipped(false);
			}

			if (!anim->animation->flip && vel->x < 0) {
				anim->animation->setFlipped(true);
			}
		}

		//update animation position
		auto hitbox = scene->getComponent<Hitbox>(entry)->rect;

		hitbox.top  += trans->y;
		hitbox.left += trans->x;

		auto width = toMeters(anim->animation->targetDim.x);
		auto height = toMeters(anim->animation->targetDim.y);
		auto center = sf::Vector2f(hitbox.left + hitbox.width / 2.0f, hitbox.top + hitbox.height / 2.0f);
		anim->animation->setPosition(sf::Vector2f(toPixels(center.x - width / 2.0f), toPixels(hitbox.top + hitbox.height - height)));
		anim->animation->update();
	}
};