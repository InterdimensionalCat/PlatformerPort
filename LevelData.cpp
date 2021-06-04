#include "stdafx.h"
#include "LevelData.h"
#include "Scene.h"
#include "Player.h"
#include "Goal.h"
#include "Actor.h"
#include "MovingPlatform.h"

void loadLevel1Actors(std::vector<std::shared_ptr<Actor>>& actors) {
	actors.push_back(std::make_shared<MovingPlatform>(toMeters(384), toMeters(192), toMeters(200), PlatformType::Falling));
	actors.push_back(std::make_shared<MovingPlatform>(toMeters(704), toMeters(192), toMeters(200), PlatformType::Falling));
	actors.push_back(std::make_shared<MovingPlatform>(toMeters(1024), toMeters(192), toMeters(200), PlatformType::Falling));

	actors.push_back(std::make_shared<MovingPlatform>(toMeters(0), toMeters(184), toMeters(200), PlatformType::Vertical));
	actors.push_back(std::make_shared<MovingPlatform>(toMeters(1408), toMeters(590), toMeters(250), PlatformType::Vertical));
	actors.push_back(std::make_shared<MovingPlatform>(toMeters(310), toMeters(448), toMeters(640), PlatformType::Horizontal));
}

void LevelData::load() {
	auto width = levelWidth;
	auto height = levelHeight;

	std::vector<std::vector<sf::Color>> colormap;

	colormap.resize((size_t)width);
	for (auto& column : colormap) {
		column.resize((size_t)height);
	}

	for (uint32_t y = 0; y < height; y++) {
		for (uint32_t x = 0; x < width; x++) {
			auto colordat = mapData.getColorAtPixel(x, y);
			colormap.at(x).at(y) = colordat;
			if (colordat.toInteger() == 0x0000FFFF) {
				Logger::get() << "Player is at: " << x << ", " << y << "\n";
				//playerSpawnPos = sf::Vector2f((float)x, (float)y);
				playerSpawnPos = sf::Vector2f((float)0, (float)0);
			}

			if (colordat.toInteger() == 0x020202FF) {
				Logger::get() << "Goal is at: " << x << ", " << y << "\n";
				goalPos = sf::Vector2f((float)x, (float)y);
			}
		}
	}
}

std::vector<std::shared_ptr<Actor>> LevelData::loadActors(Scene* scene) const {
	std::vector<std::shared_ptr<Actor>> actors;

	//load player first
	auto player = std::make_shared<Player>(*this);
	player->logic->input = std::shared_ptr<InputHandle>(scene->input);
	actors.push_back(player);
	actors.push_back(std::make_shared<Goal>(*this, scene));

	if (levelname == "level_1") {
		loadLevel1Actors(actors);
	}

	return actors;
}
