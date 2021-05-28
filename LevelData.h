#pragma once

class LevelData
{
public:
	LevelData(const std::string& levelname, const std::string& tilesetname, const std::string& parallaxTextureName, 
		const float levelWidth, const float levelHeight,
		const float baseScrollPercent, const float scrollGrowth)
		: levelname(levelname), tilesetname(tilesetname), parallaxTextureName(parallaxTextureName), 
		levelWidth(levelWidth), levelHeight(levelHeight),
		baseScrollPercent(baseScrollPercent), scrollGrowth(scrollGrowth)  {}
	std::string levelname;
	std::string tilesetname;
	std::string parallaxTextureName;
	float levelWidth;
	float levelHeight;
	float baseScrollPercent;
	float scrollGrowth;
};

