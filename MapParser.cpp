#include "stdafx.h"
#include "MapParser.h"
#include "Scene.h"
#include "TileMap.h"
#include "ActorData.h"
#include "Player.h"
#include "PlayerLogic.h"
#include "PlayerGraphics.h"
#include "Goal.h"
#include "MovingPlatform.h"
#include "Boop.h"
#include "Parallax.h"

#include "Camera.h"

void MapParser::parseMap(const std::string& mapName, Scene& scene) {

	std::filesystem::path mappath(std::filesystem::current_path());
	mappath /= "resources";
	mappath /= "maps";
	mappath /= mapName;
	mappath += ".json";

	tson::Tileson t;
	std::unique_ptr<tson::Map> map = t.parse(mappath);

	if (map->getStatus() == tson::ParseStatus::OK)
	{

		float widthPixels = toPixels((float)map->getSize().x);
		float heightPixels = toPixels((float)map->getSize().y);

		for (auto& layer : map->getLayers())
		{
			if (layer.getType() == tson::LayerType::ObjectGroup)
			{
				loadActors(layer, scene);
			}

			if (layer.getType() == tson::LayerType::TileLayer) {
				auto& data = layer.getTileData();
				scene.tilemap->loadMap(*map, data);
			}
		}

		loadParallax(*map, scene);

		scene.camera = std::make_unique<Camera>(widthPixels, heightPixels, scene.actors.at(0));
	}
	else //Error occured
	{
		Logger::get() << "Map loading error occured: " << map->getStatusMessage() << "\n";
		throw std::exception();
	}

}

void MapParser::loadParallax(tson::Map& map, Scene& scene) {
	auto parallaxname = std::any_cast<std::string>(map.getProp("BackgroundName")->getValue());

	auto baseX = std::any_cast<float>(map.getProp("baseParallaxX")->getValue());
	auto baseY = std::any_cast<float>(map.getProp("baseParallaxY")->getValue());

	auto growthX = std::any_cast<float>(map.getProp("growthParallaxX")->getValue());
	auto growthY = std::any_cast<float>(map.getProp("growthParallaxY")->getValue());

	scene.parallaxEngine = std::make_unique<Parallax>(parallaxname, baseX, baseY, growthX, growthY);
}

json propertiesToJson(tson::Object& obj) {
	json val;

	tson::PropertyCollection& properties = obj.getProperties();

	auto& x = obj.getPosition().x;
	auto& y = obj.getPosition().y;

	val["x"] = x;
	val["y"] = y;

	for (auto& property : properties.get()) {
		switch (property->getType()) {
		case tson::Type::Boolean:
			val[property->getName()] = std::any_cast<bool>(property->getValue());
			break;
		case tson::Type::Float:
			val[property->getName()] = std::any_cast<float>(property->getValue());
			break;
		case tson::Type::Int:
			val[property->getName()] = std::any_cast<int>(property->getValue());
			break;
		case tson::Type::String:
			val[property->getName()] = std::any_cast<std::string>(property->getValue());
			break;
		default:
			val[property->getName()] = std::any_cast<std::string>(property->getValue());
		}
	}

	return val;
}

void MapParser::loadActors(tson::Layer& layer, Scene& scene) {

	auto players = layer.getObjectsByName("Player");

	auto& player = players.at(0);
	auto& actors = scene.actors;

	auto playerJson = propertiesToJson(player);

	auto dat = ActorData(playerJson, &scene);

	actors.push_back(std::make_shared<Player>(dat));

	for (auto& obj : layer.getObjects()) {
		if (obj.getName() == "Goal") {
			ActorData data(propertiesToJson(obj), &scene);
			actors.push_back(std::make_shared<Goal>(data));
		}

		if (obj.getName() == "MovingPlatform") {
			ActorData data(propertiesToJson(obj), &scene);
			actors.push_back(std::make_shared<MovingPlatform>(data));
		}

		if (obj.getName() == "Boop") {
			ActorData data(propertiesToJson(obj), &scene);
			actors.push_back(std::make_shared<Boop>(data));
		}
	}
}
