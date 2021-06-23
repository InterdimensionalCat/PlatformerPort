#pragma once
#include "System.h"
#include "SFML/Graphics.hpp"

class DrawSprites : public System<SpriteDrawable> {
public:
	DrawSprites(Scene* scene, std::shared_ptr<Window> window) : System(scene), window(window) {}

	void excecutionFunction(std::shared_ptr<ActorEntry> entry) override {
		auto sprite = scene->getComponent<SpriteDrawable>(entry);
		auto trans =  scene->getComponent<Transform>(entry);
		sf::Sprite& spr = *sprite->spr;
		if (trans != nullptr) {
			spr.setPosition(toPixels(trans->x), toPixels(trans->y));
		}
		spr.setTexture(sprite->tex->getTexture());

		window->window->draw(spr);
	}
private:
	std::shared_ptr<Window> window;
};