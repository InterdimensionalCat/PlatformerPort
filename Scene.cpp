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
	input = keyboard;//std::make_shared<KeyboardInput>();
	window->registerWindowEventListener(input);

	tilemap = std::make_shared<Tilemap>();

	audio = std::make_shared<AudioEngine>();

	loadLevel("newLevel_1");

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

					if (actorname != "Player") continue;

					auto variantNameItr = std::find_if(obj.getProperties().begin(), obj.getProperties().end(), [](const tmx::Property val) {
						return val.getName() == "VariantName";
						});
					std::string variantname = "";
					if (variantNameItr != obj.getProperties().end()) {
						variantname = variantNameItr->getStringValue();
					}

					spawnActor(actorname, variantname, obj);
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

}

void Scene::loadActors() {
	//auto testactor = spawnActor("test", "test");
	auto player = spawnActor("Player", "");
	assignComponent<PlayerFlag>(player);
	assignComponent<Transform>(player, toMeters(100.0f), toMeters(100.0f));
	assignComponent<Velocity>(player, 0.0f, 0.0f);
	assignComponent<InputController>(player, std::static_pointer_cast<Input>(input));
	assignComponent<PhysicsProperties>(player, (1.0f/1.05f), toMeters(0.5f));
	assignComponent<HorzMovable>(player, toMeters(0.6f));
	assignComponent<TileCollidable>(player);
	assignComponent<Hitbox>(player, sf::FloatRect(0, 0, toMeters(64 - 25), toMeters(64 - 10)));
	assignComponent<StateController>(player, ActionState::Airborne);
	assignComponent<StateAccelerationValues>(player, toMeters(0.3f), toMeters(1.0f), toMeters(0.3f) * 3.0f, toMeters(0.3f) * 2.0f);
	assignComponent<StateFrictionValues>(player, (1.0f/1.05f), (1.0f/1.008f), (1.0f / 1.05f), (1.0f / 1.05f));

	std::shared_ptr<std::map<ActionState, AnimationEntry>> map = std::make_shared<std::map<ActionState, AnimationEntry>>();

	map->emplace(ActionState::Airborne,    AnimationEntry("astronautAirborne", sf::Vector2i(64, 64), sf::Vector2f(toPixels(1.0f), toPixels(1.0f)), 0, 4, 12));
	map->emplace(ActionState::GroundStill, AnimationEntry("astronautIdle", sf::Vector2i(45, 90), sf::Vector2f(45.0f, 90.0f), 0, 12, 12));
	map->emplace(ActionState::GroundMove,  AnimationEntry("astronautRun", sf::Vector2i(48, 96), sf::Vector2f(48.0f, 96.0f), 0, 12, 15));
	map->emplace(ActionState::GroundTurn,  AnimationEntry("astronautTurn", sf::Vector2i(60, 82), sf::Vector2f(60, 82), 0, 1, 60));

	assignComponent<AnimStateMap>(player, map, ActionState::Airborne);
	assignComponent<Animatable>(player, std::make_shared<Animation>(map->at(ActionState::Airborne), true));
	assignComponent<Turnable>(player, toMeters(2.0f));
	assignComponent<AirborneState>(player, 0, 5);
	assignComponent<TileCollisionEventListener>(player);
	assignComponent<Jumpable>(player, toMeters(15.0f), 0.83f, 0.51f);
	assignComponent<BoundedVelocity>(player, toMeters(6.5f), toMeters(35.0f));
	assignComponent<BoundByMapBounds>(player);
	assignComponent<MapBoundCollisionListener>(player);


	Logger::get() << createJsonFromActor(player)->dump(2) << "\n";

	auto flag = getComponent<PlayerFlag>(player);
	std::shared_ptr<json> file = std::make_shared<json>();
	for (auto func : generateJsonFunctions) {
		func(this, player, file);
	}

	Logger::get() << file->dump(2) << "\n";

	//saveActorJsonToFile(file, "Player");

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

void Scene::update() {
	input->update(*window);

	if (!window->updateInput()) {
		Settings::setSetting<bool>("running", false);
		return;
	}

	while (!events.empty()) {
		events.front()->excecute(this);
		events.pop();
	}

	//auto actor = spawnActor("test", "test");
	//assignComponent<Transform>(actor, toMeters(100.0f), toMeters(100.0f));

	UpdateAccelByState(this).excecute();
	UpdateFrcByState(this).excecute();
	HorzMove(this).excecute();
	JumpInput(this).excecute();
	GenericStateDriver(this).excecute();
	ApplyGravity(this).excecute();
	ApplyFriction(this).excecute();
	TilemapCollision(this, tilemap).excecute();
	BindMovement(this).excecute();
	UpdatePositions(this).excecute();
	BindPositionByMapBounds(this).excecute();
	CheckPlayerFallOffMap(this).excecute();

	for (auto entry : SceneView<PlayerFlag, Transform>(this)) {
		//Logger::get() << getComponent<Transform>(entry)->y << "\n";
	}



	camera->updateWindow(*window);
	parallaxEngine->move(*camera);
}

void Scene::draw(const float interpol) {

	StateAnimationDriver(this).excecute();
	UpdateSpritePos(this).excecute();

	window->preRender(interpol);

	parallaxEngine->draw(*window);
	tilemap->draw(*window);

	DrawAnimations(this, window).excecute();
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

	Logger::get() << entry->getData()->get()->dump(2) << "\n";

	for (auto func : generateFromJsonFunctions) {
		func(this, entry, entry->getData()->get());
	}

	return entry;
}

void Scene::despawnActor(std::shared_ptr<ActorEntry> entry) {
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