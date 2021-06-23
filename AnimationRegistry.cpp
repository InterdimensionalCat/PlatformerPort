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
	registerAnimation("PlayerIdle", Animation("astronautIdle", sf::Vector2i(45, 90), sf::Vector2f(45.0f, 90.0f), 0, 12, 12));
	registerAnimation("PlayerRun", Animation("astronautRun" , sf::Vector2i(48, 96), sf::Vector2f(48.0f, 96.0f), 0, 12, 15));
	registerAnimation("PlayerJump", Animation("astronautAirborne", sf::Vector2i(64, 64), sf::Vector2f(toPixels(1.0f), toPixels(1.0f)), 0, 4, 12));
	//registerAnimation("PlayerRun",  Animation("PlayerIdleMap", sf::Vector2i(64, 64), sf::Vector2f(toPixels(1.0f), toPixels(1.0f)), 7, 2, 6));
	registerAnimation("PlayerTurn", Animation("astronautTurn", sf::Vector2i(60, 82), sf::Vector2f(60, 82), 0, 1, 60));
}

void clearAnimRegistry() {
	animMap.clear();
}
