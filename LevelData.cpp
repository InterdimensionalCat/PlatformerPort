#include "stdafx.h"
#include "LevelData.h"
#include "Scene.h"
#include "Player.h"
#include "Goal.h"
#include "Actor.h"
#include "MovingPlatform.h"
#include "Boop.h"
#include "PlayerLogic.h"

#pragma warning( push )
#pragma warning( disable : 4244 )

void loadLevel1Actors(std::vector<std::shared_ptr<Actor>>& actors, const LevelData& data) {
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(384), toMeters(192), toMeters(200), PlatformType::Falling));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(704), toMeters(192), toMeters(200), PlatformType::Falling));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(1024), toMeters(192), toMeters(200), PlatformType::Falling));

	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(0), toMeters(184), toMeters(200), PlatformType::Vertical));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(1408), toMeters(590), toMeters(300), PlatformType::Vertical));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(310), toMeters(448), toMeters(640), PlatformType::Horizontal));

	actors.push_back(std::make_shared<Boop>(data, toMeters(448),    toMeters(1152), toMeters(1.5f), toMeters(128), false));
	actors.push_back(std::make_shared<Boop>(data, toMeters(448+64), toMeters(1152), toMeters(1.5f), toMeters(64 ), false));
	actors.push_back(std::make_shared<Boop>(data, toMeters(1216),   toMeters(1152), toMeters(1.5f), toMeters(128), false));
}

void loadLevel2Actors(std::vector<std::shared_ptr<Actor>>& actors, const LevelData& data) {
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(1792), toMeters(128), toMeters(500), PlatformType::Vertical));


	actors.push_back(std::make_shared<Boop>(data, toMeters(1792 + 32), toMeters(128 - 64), toMeters(0.7f), toMeters(16), false));
	actors.push_back(std::make_shared<Boop>(data, toMeters(2048), toMeters(128), toMeters(2.4f), toMeters(64), false));
	actors.push_back(std::make_shared<Boop>(data, toMeters(2304), toMeters(128), toMeters(2.5f), toMeters(64), false));
	actors.push_back(std::make_shared<Boop>(data, toMeters(2688), toMeters(512), toMeters(0.7f), toMeters(50), false, 0.0f));
	actors.push_back(std::make_shared<Boop>(data, toMeters(3840), toMeters(832), toMeters(0.7f), toMeters(64), true));
	actors.push_back(std::make_shared<Boop>(data, toMeters(5824), toMeters(832), toMeters(0.0f), toMeters(0.0f), true));
}

void loadLevel3Actors(std::vector<std::shared_ptr<Actor>>& actors, const LevelData& data) {
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(832), toMeters(10240 - 640), toMeters(576), PlatformType::Vertical));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(1152), toMeters(9408), toMeters(200), PlatformType::Falling));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(1024), toMeters(9408), toMeters(200), PlatformType::Falling));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(896 ), toMeters(9408), toMeters(200), PlatformType::Falling));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(768 ), toMeters(9408), toMeters(200), PlatformType::Falling));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(640 ), toMeters(9408), toMeters(200), PlatformType::Falling));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(512 ), toMeters(9408), toMeters(200), PlatformType::Falling));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(384 ), toMeters(9408), toMeters(200), PlatformType::Falling));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(256 ), toMeters(9408), toMeters(200), PlatformType::Falling));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(128 ), toMeters(9408), toMeters(200), PlatformType::Falling));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(0   ), toMeters(9408), toMeters(200), PlatformType::Falling));

	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(0), toMeters(8256 - 640), toMeters(640), PlatformType::Vertical));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(1152), toMeters(7616 - 640), toMeters(640), PlatformType::Vertical));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(384), toMeters(6848 - 320), toMeters(320), PlatformType::Vertical));
	actors.push_back(std::make_shared<MovingPlatform>(data, toMeters(0), toMeters(6492 - 640), toMeters(640), PlatformType::Vertical));

	actors.push_back(std::make_shared<Boop>(data, 13.5f, toMeters(9856 - 13), toMeters(5.5f + 0.3f), 4, false, 0));
	actors.push_back(std::make_shared<Boop>(data, 13.5f, toMeters(9728 - 13), toMeters(5.3f + 0.3f), 4, false, 0));
	actors.push_back(std::make_shared<Boop>(data, 13.5f, toMeters(9600 - 13), toMeters(5.1f + 0.3f), 4, false, 0));

	actors.push_back(std::make_shared<Boop>(data, toMeters(1152), toMeters(8512), toMeters(0  ), toMeters(0  ), true));
	actors.push_back(std::make_shared<Boop>(data, toMeters(256 ), toMeters(8448), toMeters(0  ), toMeters(0  ), false, 0));
	actors.push_back(std::make_shared<Boop>(data, toMeters(576 ), toMeters(8448), toMeters(0  ), toMeters(0  ), false, 0));

	actors.push_back(std::make_shared<Boop>(data, toMeters(1088), toMeters(7552), toMeters(0  ), toMeters(0  ), true, toMeters(0.4f)));

	actors.push_back(std::make_shared<Boop>(data, toMeters(960 ), toMeters(7040), toMeters(5.0), toMeters(30 ), false, 0));
	actors.push_back(std::make_shared<Boop>(data, toMeters(832 ), toMeters(6976), toMeters(5.0), toMeters(30 ), false, 0));
	actors.push_back(std::make_shared<Boop>(data, toMeters(640 ), toMeters(6912), toMeters(5.0), toMeters(30 ), false, 0));

	actors.push_back(std::make_shared<Boop>(data, toMeters(896 ), toMeters(5440), toMeters(0  ), toMeters(0  ), true));
	actors.push_back(std::make_shared<Boop>(data, toMeters(960 ), toMeters(2800), toMeters(0  ), toMeters(0  ), false, 0));
	actors.push_back(std::make_shared<Boop>(data, toMeters(512 ), toMeters(448 ), toMeters(0  ), toMeters(0  ), false, toMeters(0.1f)));
}

#pragma warning( pop )

LevelData::LevelData(const std::string& levelname, const std::string& tilesetname, const std::string& parallaxTextureName,
	const float levelWidth, const float levelHeight,
	const float baseScrollPercent, const float scrollGrowth, const float scrollY, const float yGrowth)
	: levelname(levelname), tilesetname(tilesetname), parallaxTextureName(parallaxTextureName),
	levelWidth(levelWidth), levelHeight(levelHeight),
	baseScrollPercent(baseScrollPercent), scrollGrowth(scrollGrowth),
	mapData(Image(levelname)), scrollY(scrollY), yGrowth(yGrowth) {
	load();
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
				playerSpawnPos = sf::Vector2f((float)x, (float)y);
				//playerSpawnPos = sf::Vector2f((float)0, (float)0);
			}

			if (colordat.toInteger() == 0x020202FF) {
				Logger::get() << "Goal is at: " << x << ", " << y << "\n";
				goalPos = sf::Vector2f((float)x, (float)y);
			}
		}
	}
}

std::vector<std::shared_ptr<Actor>> LevelData::loadActors(Scene* scene) const {
	this->scene = scene;
	std::vector<std::shared_ptr<Actor>> actors;

	//load player first
	auto player = std::make_shared<Player>(*this);
	player->logic->input = std::shared_ptr<KeyboardInput>(scene->input);
	actors.push_back(player);
	actors.push_back(std::make_shared<Goal>(*this, scene));

	if (levelname == "level_F1") {
		loadLevel1Actors(actors, *this);
	}

	if (levelname == "level_F2") {
		loadLevel2Actors(actors, *this);
	}

	if (levelname == "level_F3") {
		loadLevel3Actors(actors, *this);
	}

	return actors;
}
