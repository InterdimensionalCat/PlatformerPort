#include "stdafx.h"
#include "Scene.h"
#include "Tilemap.h"
#include "TileRegistry.h"
#include "Window.h"
#include "Parallax.h"
#include "Camera.h"
#include "Input.h"
#include "PhysicsEngine.h"
#include "AnimationRegistry.h"
#include "AudioRegistry.h"
#include "AudioEngine.h"
#include "Actor.h"

#include "System.h"
#include "PhysicsSystems.h"
#include "GraphicsSystems.h"
#include "InputSystems.h"

int componentCounter = 0;

Scene::Scene() {
	//init();

	RegisterTiles();
	RegisterAudio();

	window = std::make_shared<Window>();
	input = std::make_shared<KeyboardInput>();
	window->registerWindowEventListener(input);

	parallaxEngine = std::make_shared<Parallax>("level1bg", 1.1f, 1.3f, 0.05f, 0.1f);
	tilemap = std::make_shared<Tilemap>();

	std::filesystem::path mappath(std::filesystem::current_path());
	mappath /= "resources";
	mappath /= "maps";
	mappath /= "newLevel_1";
	mappath += ".tmx";

	float widthPixels = 0;
	float heightPixels = 0;

	tmx::Map map;
	if (map.load(mappath.string())) {


		widthPixels = (float)(toPixels((float)map.getLayers().at(0)->getSize().x));
		heightPixels = (float)(toPixels((float)map.getLayers().at(0)->getSize().y));

		for (auto& layer : map.getLayers())
		{
			if (layer->getType() == tmx::Layer::Type::Object) {

				auto& objlayer = layer->getLayerAs<tmx::ObjectGroup>();
				//loadActors(objlayer, scene);
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
						tilemap->loadMap(toMeters(widthPixels), toMeters(heightPixels), tiles, set);
						continue;
					}
				}
			}
		}
	}


	loadActors();

	for (auto entry : SceneView<PlayerFlag>(this)) {
		camera = std::make_shared<Camera>(widthPixels, heightPixels, entry, this);
	}

	audio = std::make_shared<AudioEngine>();
	audio->playMusic("FlatZone", 10.0f);
}

Scene::~Scene() {
	clearAudioRegistry();

	//auto& pool = componentPools.at(getComponentId<InputController>());
	//for (size_t i = 0; i < pool.size(); i++) {
	//	if (!actors.at(i)->hasComponent(getComponentId<InputController>())) continue;
	//	InputController* component = static_cast<InputController*>(pool.at(i));
	//	delete component;
	//}
	////pool.data = nullptr;

	//auto& pool = componentPools.at(getComponentId<SpriteDrawable>());
	//for (size_t i = 0; i < pool.size(); i++) {
	//	if (!actors.at(i)->hasComponent(getComponentId<SpriteDrawable>())) continue;
	//	SpriteDrawable* component = static_cast<SpriteDrawable*>(pool.at(i));
	//	delete component->spr;
	//	delete component->tex;
	//}
	////pool.data = nullptr;

	componentPools.clear();

}

void Scene::loadActors() {
	spawnActor("test", "test");
	auto player = spawnActor("Player", "");
	assignComponent<PlayerFlag>(player);
	assignComponent<Transform>(player, toMeters(100.0f), toMeters(100.0f));
	assignComponent<Velocity>(player, 0.0f, 0.0f);
	assignComponent<InputController>(player, std::static_pointer_cast<Input>(input));
	assignComponent<PhysicsProperties>(player, 0.0f, toMeters(0.5f), 0.0f);
	assignComponent<HorzMovable>(player, toMeters(0.6f));
	assignComponent<TileCollidable>(player);
	assignComponent<SpriteDrawable>(player, std::make_shared<sf::Sprite>(), std::make_shared<Texture>("astronautTurn"));
	assignComponent<Hitbox>(player, sf::FloatRect(0, 0, 1.0f, 1.0f));

	spawnActor("test", "test");

	//removeComponent<InputController>(player);
	//removeComponent<SpriteDrawable>(player);
}

void Scene::update() {
	input->update(*window);

	if (!window->updateInput()) {
		Settings::setSetting<bool>("running", false);
		return;
	}

	auto actor = spawnActor("test", "test");
	assignComponent<Transform>(actor, toMeters(100.0f), toMeters(100.0f));

	HorzMove(this).excecute();
	GenericStateDriver(this).excecute();
	ApplyGravity(this).excecute();
	TilemapCollision(this, tilemap).excecute();
	UpdatePositions(this).excecute();

	for (auto entry : SceneView<PlayerFlag, Transform>(this)) {
		//Logger::get() << getComponent<Transform>(entry)->y << "\n";
	}



	camera->updateWindow(*window);
	parallaxEngine->move(*camera);
}

void Scene::draw(const float interpol) {

	window->preRender(interpol);

	parallaxEngine->draw(*window);
	tilemap->draw(*window);

	DrawSprites(this, window).excecute();

	window->postRender();
}

std::shared_ptr<ActorEntry> Scene::spawnActor(const std::string& typeName, const std::string& variantName) {
	if (!freeActors.empty()) {
		auto recycledEntry = freeActors.back();
		freeActors.pop_back();
		recycledEntry->createNewEntry(typeName, variantName);
		return recycledEntry;
	}

	if (maxActors <= actors.size()) {
		maxActors *= 2;

		for (auto& pool : componentPools) {
			pool->resize(maxActors);
		}
	}

	actors.push_back(std::make_shared<ActorEntry>(actors.size()));
	auto entry = actors.back();
	entry->createNewEntry(typeName, variantName);
	return entry;
}