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

#include "Animation.h"

#include "ActorData.h"


int componentCounter = 0;

Scene::Scene() {
	//init();

	registerComponentsFunction();

	RegisterTiles();
	RegisterAudio();

	window = std::make_shared<Window>();

	tilemap = std::make_shared<Tilemap>();

	audio = std::make_shared<AudioEngine>();

	loadLevel("newLevel_1");

	for (auto& [name, id] : System::nameidmap) {
		systems.push_back(std::move(System::createSystem(this, name)));
	}

	std::sort(systems.begin(), systems.end(), [](const auto a, const auto b) {
		return a->getType() < b->getType();
		}
	);

	for (auto system : systems) {
		//Logger::get() << system->toString() << "\n";
	}

	//audio->playMusic("FlatZone", 10.0f);
}

void Scene::loadLevel(const std::string& levelname) {

	actors.clear();
	freeActors.clear();
	componentPools.clear();

	currentlevelname = levelname;
	std::filesystem::path mappath(std::filesystem::current_path());
	mappath /= "resources";
	mappath /= "maps";
	mappath /= levelname;
	mappath += ".tmx";

	float widthPixels = 0;
	float heightPixels = 0;

	tmx::Map map;
	if (map.load(mappath.string())) {


		widthPixels = (float)(toPixels((float)map.getLayers().at(0)->getSize().x));
		heightPixels = (float)(toPixels((float)map.getLayers().at(0)->getSize().y));

		Scene::mapwidth = toMeters(widthPixels);
		Scene::mapheight = toMeters(heightPixels);

		for (auto& layer : map.getLayers())
		{
			if (layer->getType() == tmx::Layer::Type::Object) {

				auto& objlayer = layer->getLayerAs<tmx::ObjectGroup>();
				for (auto& obj : objlayer.getObjects()) {
					auto actorname = obj.getName();

					auto variantNameItr = std::find_if(obj.getProperties().begin(), obj.getProperties().end(), [](const tmx::Property val) {
						return val.getName() == "VariantName";
						});
					std::string variantname = "";
					if (variantNameItr != obj.getProperties().end()) {
						variantname = variantNameItr->getStringValue();
					}

					//spawnActor(actorname, variantname);

					spawnActor(actorname, variantname, obj);

					//if (actorname == "Player") {
					//	spawnActor(actorname, variantname, obj);
					//}
					//else {
					//	spawnActor(actorname, variantname);
					//}

				}
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


	//loadActors();

	for (auto entry : SceneView<PlayerFlag>(this)) {
		camera = std::make_shared<Camera>(widthPixels, heightPixels, entry, this);
	}

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


	parallaxEngine = std::make_shared<Parallax>(parallaxname, baseX, baseY, growthX, growthY);
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
	actors.clear();
	freeActors.clear();

	while (!preevents.empty()) {
		preevents.pop();
	}

	while (!postevents.empty()) {
		postevents.pop();
	}

}

void Scene::loadActors() {
	//auto testactor = spawnActor("test", "test");
	auto player = spawnActor("MovingPlatform", "");
	assignComponent<Transform>(player, toMeters(70.0f), 32.0f);
	assignComponent<Velocity>(player, 0.0f, 0.0f);

	assignComponent<Hitbox>(player, sf::FloatRect(0, 0, toMeters(128.0f), toMeters(20.0f)));
	assignComponent<HitboxExtension>(player, sf::FloatRect(0, 0, toMeters(128.0f), toMeters(60.0f)));

	assignComponent<MapBoundCollisionListener>(player);
	assignComponent<CircularConstrainedMovement>(player, 4.0f, toMeters(100.0f), toMeters(100.0f));
	assignComponent<ActorCollidable>(player, "platform");
	assignComponent<PlatformTypeable>(player, PlatformType::Horizontal);
	assignComponent<PlatformTolerence>(player, toMeters(4.0f), toMeters(20.0f));
	assignComponent<CircularMovable>(player, toMeters(3.0f), 1.0f, 0.0f);
	assignComponent<SpriteDrawable>(player, std::make_shared<sf::Sprite>(), std::make_shared<Texture>("movingTileAstro"));


	std::shared_ptr<json> file = createJsonFromActor(player);

	Logger::get() << file->dump(2) << "\n";

	saveActorJsonToFile(file, "MovingPlatform");

	//spawnActor("test", "test");

	//removeComponent<InputController>(player);
	//removeComponent<SpriteDrawable>(player);
}


void Scene::saveActorJsonToFile(std::shared_ptr<json> data, const std::string& filename) {
	fs::path filepath(fs::current_path());
	filepath /= "resources";
	filepath /= "actors";
	filepath /= filename;
	filepath += ".json";

	std::shared_ptr<json> olddata = std::make_shared<json>();

	if (fs::exists(filepath)) {
		std::fstream readfile;
		readfile.open(filepath, std::fstream::in);

		try {
			olddata->parse(readfile);
		}
		catch (json::parse_error& e)
		{
			Logger::get() << e.what() << "\n";
			Logger::get() << "file " + filename + " does not appear to have existing json data.\n";
		}
	}

	olddata->update(*data);

	std::ofstream file;
	file.open(filepath, std::ofstream::out);

	file.clear();
	file << olddata->dump(2);
	file.close();
}

void Scene::runSystemsInRange(const SystemType begin, const SystemType end) {
	auto beginitr = std::find_if(systems.begin(), systems.end(), [begin](const auto a) {
		return (a->getType() >= begin);
		});
	auto enditr = std::find_if(systems.begin(), systems.end(), [end](const auto a) {
		return (a->getType() >= end);
		});

	std::for_each(beginitr, enditr,
		[](auto a) {
				a->excecute();
			}
	);
}

void Scene::handlePreEvents() {
	std::vector<std::string> excecutedevents;

	while (!preevents.empty()) {

		//TODO: events that are not local to a 
		//specific actor and are scenewide should be 
		//agregated into 1 event call
		if (std::find(excecutedevents.begin(), excecutedevents.end(), preevents.front()->getName())
			== excecutedevents.end()) {

			preevents.front()->excecute(this);
			excecutedevents.push_back(preevents.front()->getName());
		}
		preevents.pop();
	}
}

void Scene::handlePostEvents() {
	std::vector<std::string> excecutedevents;

	while (!postevents.empty()) {

		//TODO: events that are not local to a 
		//specific actor and are scenewide should be 
		//agregated into 1 event call
		if (std::find(excecutedevents.begin(), excecutedevents.end(), postevents.front()->getName())
			== excecutedevents.end()) {

			postevents.front()->excecute(this);
			excecutedevents.push_back(postevents.front()->getName());
		}
		postevents.pop();
	}
}

void Scene::update() {

	if (!window->updateInput()) {
		Settings::setSetting<bool>("running", false);
		return;
	}

	handlePreEvents();

	runSystemsInRange(SystemType::PreUpdate, SystemType::PreGraphics);

	camera->updateWindow(*window);
	parallaxEngine->move(*camera);

	handlePostEvents();
}

void Scene::draw(const float interpol) {

	//StateAnimationDriver(this).excecute();
	//UpdateSpritePos(this).excecute();

	runSystemsInRange(SystemType::PreGraphics, SystemType::Graphics);

	window->preRender(interpol);

	parallaxEngine->draw(*window);
	tilemap->draw(*window);

	for (auto& shape : debugShapes) {
		//window->window->draw(shape);
	}

	debugShapes.clear();

	runSystemsInRange(SystemType::Graphics, SystemType::Count);

	//DrawAnimations(this).excecute();
	//DrawSprites(this).excecute();

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

	//Logger::get() << entry->getData()->get()->dump(2) << "\n";

	for (auto func : generateFromJsonFunctions) {
		func(this, entry, entry->getData()->get());
	}

	return entry;
}


std::shared_ptr<ActorEntry> Scene::spawnActor(const std::string& typeName, const std::string& variantName, const tmx::Object& object) {
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
	entry->createNewEntry(typeName, variantName, object);

	//Logger::get() << entry->getData()->get()->dump(2) << "\n";

	for (auto func : generateFromJsonFunctions) {
		func(this, entry, entry->getData()->get());
	}

	return entry;
}

void Scene::despawnActor(std::shared_ptr<ActorEntry> entry) {

	Logger::get() << "despawning actor:" + std::to_string(entry->getIndex()) + "\n";

	if (entry->isRespawnable()) {
		entry->setActive(false);
	}
	else {
		entry->resetEntry();
		for (auto pool : componentPools) {
			pool->removeComponent(entry->getIndex());
		}
		freeActors.push_back(entry);
	}
}

void Scene::respawnActor(std::shared_ptr<ActorEntry> entry) {
	if (!entry->isRespawnable()) {
		return;
	}

	entry->setActive(true);

	for (auto func : generateFromJsonFunctions) {
		func(this, entry, entry->getData()->get());
	}
}


std::shared_ptr<json> Scene::createJsonFromActor(std::shared_ptr<ActorEntry> entry) {
	std::shared_ptr<json> file = std::make_shared<json>();
	for (auto func : generateJsonFunctions) {
		func(this, entry, file);
	}
	return file;
}