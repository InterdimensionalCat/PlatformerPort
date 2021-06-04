#include "stdafx.h"
#include "AnimationRegistry.h"
#include "Animation.h"
#include <exception>

static std::map<std::string, Animation> animMap = std::map<std::string, Animation>();

Animation getAnimFromName(const std::string& name) {
	auto found = animMap.find(name);
	if (found != animMap.end()) {
		return found->second;
	}
	else {
		throw std::exception();
		return found->second;
	}
}

void registerAnimation(const std::string& name, Animation anim) {
	animMap.emplace(name, anim);
	Logger::get() << "Animation registered with name: " << name << "\n";
}

void RegisterAnimations() {
	registerAnimation("BoopAnim",   Animation("BoopAnim",     sf::Vector2i(64, 64), sf::Vector2f(toPixels(1.0f), toPixels(1.0f)), 0, 3, 12));
	registerAnimation("PlayerIdle", Animation("PlayerIdleMap", sf::Vector2i(64, 64), sf::Vector2f(toPixels(1.0f), toPixels(1.0f)), 0, 3, 1));
	registerAnimation("PlayerJump", Animation("PlayerIdleMap", sf::Vector2i(64, 64), sf::Vector2f(toPixels(1.0f), toPixels(1.0f)), 3, 4, 6));
	registerAnimation("PlayerRun",  Animation("PlayerIdleMap", sf::Vector2i(64, 64), sf::Vector2f(toPixels(1.0f), toPixels(1.0f)), 7, 2, 6));
	registerAnimation("PlayerTurn", Animation("PlayerIdleMap", sf::Vector2i(64, 64), sf::Vector2f(toPixels(1.0f), toPixels(1.0f)), 9, 1, 6));
}

void clearAnimRegistry() {
	animMap.clear();
}
