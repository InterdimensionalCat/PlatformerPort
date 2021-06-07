#pragma once
#include "Image.h"

class Actor;
class Scene;

class LevelData
{
public:
	LevelData(const std::string& levelname, const std::string& tilesetname, const std::string& parallaxTextureName,
		const float levelWidth, const float levelHeight,
		const float baseScrollPercent, const float scrollGrowth);

	void load();
	std::vector<std::shared_ptr<Actor>> loadActors(Scene* scene) const;

	ic::Image mapData;
	std::string levelname;
	std::string tilesetname;
	std::string parallaxTextureName;
	float levelWidth;
	float levelHeight;
	float baseScrollPercent;
	float scrollGrowth;

	sf::Vector2f playerSpawnPos;
	sf::Vector2f goalPos;
	mutable Scene* scene;
};

