#include "stdafx.h"
#include "LevelRegistry.h"
#include "LevelData.h"

static std::vector<LevelData> levelDataVec;
static uint32_t levelCounter = 1;

void registerLevel(const LevelData& level) {
	levelDataVec.push_back(level);
	levelCounter++;
}

void RegisterLevels() {
	//registerLevel(LevelData("level_1", "SpriteMap1", "background", 25, 25, 1.1f, 0.07f));
	registerLevel(LevelData("level_1", "SpriteMap1", "background", 25, 25, 1.0f, 0.02f));
	registerLevel(LevelData("level_2", "SpriteMap1", "background", 100, 15, 2.1f, 0.07f));
	registerLevel(LevelData("level_3", "SpriteMap1", "background", 20, 200, 1.0f, 0.02f));
}

const LevelData& getLevelData(const uint32_t levelNum) {
	auto vecnum = levelNum == 0 ? 0 : levelNum - 1;
	return levelDataVec.at(vecnum);
}