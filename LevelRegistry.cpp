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
	registerLevel(LevelData("level_1", "SpriteMap1"));
}

const LevelData& getLevelData(const uint32_t levelNum) {
	auto vecnum = levelNum == 0 ? 0 : levelNum - 1;
	return levelDataVec.at(vecnum);
}