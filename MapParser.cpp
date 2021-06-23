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
	mappath += ".tmx";

	tmx::Map map;
	if (map.load(mappath.string())) {
			

		float widthPixels  = (float)(toPixels((float)map.getLayers().at(0)->getSize().x));
		float heightPixels = (float)(toPixels((float)map.getLayers().at(0)->getSize().y));

		//scene.tilemap = std::make_shared<Tilemap>();

		scene.actors.clear();

		for (auto& layer : map.getLayers())
		{
			if (layer->getType() == tmx::Layer::Type::Object) {

				auto& objlayer = layer->getLayerAs<tmx::ObjectGroup>();
				loadActors(objlayer, scene);
			}

			if (layer->getType() == tmx::Layer::Type::Tile) {
				auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
				auto tiles = tileLayer.getTiles();
				auto firstTileNum = tiles.at(0).ID;

				std::string tilesetname;
				for (auto& property : tileLayer.getProperties()) {
					if (property.getName() == "tilesetname") {
						tilesetname = property.getStringValue();
						break;
					}
				}

				for (size_t i = 0; i < map.getTilesets().size(); i++) {
					auto set = map.getTilesets().at(i);

					if (set.getName() == tilesetname) {
						scene.tilemap->loadMap(toMeters(widthPixels), toMeters(heightPixels), tiles, set);
						continue;
					}
				}
			}
		}

		loadParallax(map, scene);
		//scene.camera = Camera(widthPixels, heightPixels, scene.actors.at(0));
	}
	else {
		Logger::get() << "Map loading error occured\n";
		throw std::exception();
	}
}

void MapParser::loadParallax(tmx::Map& map, Scene& scene) {
	std::string parallaxname;
	float baseX;
	float baseY;
	float growthX;
	float growthY;

	for (auto& property : map.getProperties()) {
		if (property.getName() == "BackgroundName") {
			parallaxname = property.getStringValue();
		}

		if (property.getName() == "baseParallaxX") {
			baseX = property.getFloatValue();
		}

		if (property.getName() == "baseParallaxY") {
			baseY = property.getFloatValue();
		}

		if (property.getName() == "growthParallaxX") {
			growthX = property.getFloatValue();
		}

		if (property.getName() == "growthParallaxY") {
			growthY = property.getFloatValue();
		}
	}

	scene.parallaxEngine = Parallax(parallaxname, baseX, baseY, growthX, growthY);
}


json propertiesToJson(const tmx::Object& obj) {
	json val;

	auto& properties = obj.getProperties();

	auto& x = obj.getPosition().x;
	auto& y = obj.getPosition().y;

	val["x"] = x;
	val["y"] = y;

	for (auto& property : properties) {
		switch (property.getType()) {
		case tmx::Property::Type::Boolean:
			val[property.getName()] = property.getBoolValue();
			break;
		case tmx::Property::Type::Float:
			val[property.getName()] = property.getFloatValue();
			break;
		case tmx::Property::Type::Int:
			val[property.getName()] = property.getIntValue();
			break;
		case tmx::Property::Type::String:
			val[property.getName()] = property.getStringValue();
			break;
		default:
			val[property.getName()] = property.getStringValue();
		}
	}

	return val;
}

void MapParser::loadActors(tmx::ObjectGroup& layer, Scene& scene) {

	auto& actors = scene.actors;

	auto& objects = layer.getObjects();

	//find player first
	for (auto& obj : objects) {
		if (obj.getName() == "Player") {
			auto playerJson = propertiesToJson(obj);
			auto dat = ActorData(playerJson, &scene);
			//actors.push_back(std::make_shared<Player>(dat));
		}
	}

	for (auto& obj : objects) {
		if (obj.getName() == "Goal") {
			ActorData data(propertiesToJson(obj), &scene);
			//actors.push_back(std::make_shared<Goal>(data));
		}

		if (obj.getName() == "MovingPlatform") {
			ActorData data(propertiesToJson(obj), &scene);
			//actors.push_back(std::make_shared<MovingPlatform>(data));
		}

		if (obj.getName() == "Boop") {
			ActorData data(propertiesToJson(obj), &scene);
			//actors.push_back(std::make_shared<Boop>(data));
		}
	}
}
