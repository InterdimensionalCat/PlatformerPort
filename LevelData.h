#pragma once
#include "Image.h"

class Actor;
class Scene;

class LevelData
{
public:
	LevelData(const std::string& levelname, const std::string& tilesetname, const std::string& parallaxTextureName,
		const float levelWidth, const float levelHeight,
		const float baseScrollPercent, const float scrollGrowth, const float scrollY = 1.0f, const float yGrowth = 0.0f);

	void load();
	std::vector<std::shared_ptr<Actor>> loadActors(Scene* scene) const;

	Image mapData;
	std::string levelname;
	std::string tilesetname;
	std::string parallaxTextureName;
	float levelWidth;
	float levelHeight;
	float baseScrollPercent;
	float scrollGrowth;

	float scrollY; 
	float yGrowth;

	sf::Vector2f playerSpawnPos;
	sf::Vector2f goalPos;
	mutable Scene* scene;
};

