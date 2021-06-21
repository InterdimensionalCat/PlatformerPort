#pragma once

#include "Window.h"
#include "MapParser.h"
#include "ComponentPool.h"
#include "Parallax.h"
#include "Camera.h"
#include "Tilemap.h"
#include "Input.h"
#include "PhysicsEngine.h"
#include "AudioEngine.h"
#include "ActorEntry.h"

//class Tilemap;
//class Parallax;
//class Camera;
//class KeyboardInput;
//class PhysicsEngine;
//class AudioEngine;
class Actor;

struct Transform
{
	sf::Vector2f pos;
};

struct Shape
{
	sf::Color color;
};



class Scene
{
public:

	//template<typename... ComponentTypes>
	//class SceneView
	//{
	//public:
	//	SceneView(Scene& scene) : scene(&scene)
	//	{
	//		if (sizeof...(ComponentTypes) == 0)
	//		{
	//			all = true;
	//		}
	//		else
	//		{
	//			// Unpack the template parameters into an initializer list
	//			int componentIds[] = { 0, GetId<ComponentTypes>() ... };
	//			for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
	//				componentMask.set(componentIds[i]);
	//		}
	//	}

	//	struct Iterator
	//	{
	//		Iterator(Scene* scene, EntityIndex index, ComponentMask mask, bool all)
	//			: scene(scene), index(index), mask(mask), all(all) {}

	//		EntityID operator*() const
	//		{
	//			return scene->entities[index].id;
	//		}

	//		bool operator==(const Iterator& other) const
	//		{
	//			return index == other.index || index == scene->entities.size();
	//		}
	//		bool operator!=(const Iterator& other) const
	//		{
	//			return index != other.index && index != scene->entities.size();
	//		}

	//		bool ValidIndex()
	//		{
	//			return
	//				// It's a valid entity ID
	//				scene->IsEntityValid(scene->entities[index].id) &&
	//				// It has the correct component mask
	//				(all || mask == (mask & scene->entities[index].mask));
	//		}

	//		Iterator& operator++()
	//		{
	//			do
	//			{
	//				index++;
	//			} while (index < scene->entities.size() && !ValidIndex());
	//			return *this;
	//		}



	//		EntityIndex index;
	//		Scene* scene;
	//		ComponentMask mask;
	//		bool all = false;
	//	};

	//	const Iterator begin() const
	//	{
	//		int firstIndex = 0;
	//		while (firstIndex < scene->entities.size() &&
	//			(componentMask != (componentMask & scene->entities[firstIndex].mask)
	//				|| !scene->IsEntityValid(scene->entities[firstIndex].id)))
	//		{
	//			firstIndex++;
	//		}
	//		return Iterator(scene, firstIndex, componentMask, all);
	//	}

	//	const Iterator end() const
	//	{
	//		return Iterator(scene, EntityIndex(scene->entities.size()), componentMask, all);
	//	}


	//	Scene* scene = nullptr;
	//	ComponentMask componentMask;
	//	bool all = false;
	//};

	Scene() {

		auto player = spawnActor("Player", "");
		auto groundBoop = spawnActor("Boop", "grounded");

		assignComponent<Transform>(player);
		assignComponent<Transform>(groundBoop);
		assignComponent<Shape>(player);
		assignComponent<Shape>(groundBoop);

		auto jumpBoop = spawnActor("Boop", "jumper");

		assignComponent<Transform>(jumpBoop);

		auto cmp = getComponent<Transform>(jumpBoop);

		assignComponent<Shape>(jumpBoop);
		removeComponent<Shape>(jumpBoop);

		despawnActor(jumpBoop);

		jumpBoop = spawnActor("Boop", "still");

		assignComponent<Shape>(jumpBoop);



		/*EntityID triangle = NewEntity();
		Transform* transform = Assign<Transform>(triangle);
		Shape* shape = Assign<Shape>(triangle);

		EntityID circle = NewEntity();
		Assign<Shape>(circle);

		for (EntityID ent : SceneView<Transform, Shape>(*this))
		{
			Transform* shansform = this->Get<Transform>(ent);
			Shape* shape = this->Get<Shape>(ent);

		}*/

		for (auto& pool : componentPools) {
			pool.logData();
		}
	}
	~Scene() {}



	void update() {
		Settings::setSetting<bool>("running", false);
		return;
	}

	void removeActor(Actor* actor) {}

	void cleanupActors() {}

	void draw(const float interpol) {}

	void setChangeLevel() {}

	void changeLevel() {}

	void setResetLevel() {}

	void resetLevel() {}

	AudioEngine audio;

	template<typename T>
	T* assignComponent(std::shared_ptr<ActorEntry> entry) {
		auto idind = entry->getIndex();
		int componentId = getComponentId<T>();
		if (componentPools.size() <= componentId) {
			componentPools.resize(componentId + 1);
		}

		if (!componentPools.at(componentId).isValid()) {
			componentPools.at(componentId) = ComponentPool(sizeof(T), maxActors);
		}


		T* component = new (componentPools.at(componentId).at(idind)) T();
		entry->setComponent(componentId);
		return component;
	}

	template<typename T>
	T* getComponent(const std::shared_ptr<ActorEntry> entry) {
		int componentId = getComponentId<T>();
		if (!entry->hasComponent(componentId)) {
			return nullptr;
		}

		T* component = static_cast<T*>(componentPools.at(componentId).at(entry->getIndex()));
		return component;
	}

	template<typename T>
	void removeComponent(std::shared_ptr<ActorEntry> entry) {
		if (!entry->isIdValid()) {
			int componentId = getComponentId<T>();
			entry->removeComponent(componentId);
		}
	}

	void despawnActor(std::shared_ptr<ActorEntry> entry) {
		entry->resetEntry();
		freeActors.push_back(entry);
	}

	std::shared_ptr<ActorEntry> spawnActor(const std::string& typeName, const std::string& variantName) {
		if (!freeActors.empty()) {
			auto recycledEntry = freeActors.back();
			freeActors.pop_back();
			recycledEntry->createNewEntry(typeName, variantName);
			return recycledEntry;
		}

		if (maxActors <= actors.size()) {
			for (auto& pool : componentPools) {
				pool.resize((size_t)2);
			}
			maxActors *= 2;
		}

		actors.push_back(std::make_shared<ActorEntry>(actors.size()));
		auto entry = actors.back();
		entry->createNewEntry(typeName, variantName);
		return entry;
	}
	
	//template<typename T>
	//T* Assign(EntityID id)
	//{
	//	auto idind = GetEntityIndex(id);
	//	int componentId = GetId<T>();

	//	if (componentPools.size() <= componentId) // Not enough component pool
	//	{
	//		componentPools.resize(componentId + 1, nullptr);
	//	}
	//	if (componentPools[componentId] == nullptr) // New component, make a new pool
	//	{
	//		componentPools[componentId] = new ComponentPool(sizeof(T));
	//	}

	//	// Looks up the component in the pool, and initializes it with placement new
	//	T* component = new (componentPools[componentId]->get(idind)) T();

	//	// Set the bit for this component to true and return the created component
	//	entities[idind].mask.set(componentId);
	//	return component;
	//}

	/*template<typename T>
	T* Get(EntityID id)
	{
		int componentId = GetId<T>();
		if (!entities[id].mask.test(componentId))
			return nullptr;

		T* pComponent = static_cast<T*>(componentPools[componentId]->get(id));
		return pComponent;
	}*/

	//template<typename T>
	//void Remove(EntityID id)
	//{
	//	// ensures you're not accessing an entity that has been deleted
	//	if (entities[GetEntityIndex(id)].id != id)
	//		return;

	//	int componentId = GetId<T>();
	//	entities[GetEntityIndex(id)].mask.reset(componentId);
	//}

	/*void DestroyEntity(EntityID id)
	{
		EntityID newID = CreateEntityId(EntityIndex(-1), GetEntityVersion(id) + 1);
		entities[GetEntityIndex(id)].id = newID;
		entities[GetEntityIndex(id)].mask.reset();
		freeEntities.push_back(GetEntityIndex(id));
	}*/

	/*EntityID NewEntity()
	{
		if (!freeEntities.empty())
		{
			EntityIndex newIndex = freeEntities.back();
			freeEntities.pop_back();
			EntityID newID = CreateEntityId(newIndex, GetEntityVersion(entities[newIndex].id));
			entities[newIndex].id = newID;
			return entities[newIndex].id;
		}
		entities.push_back({ CreateEntityId(EntityIndex(entities.size()), 0), ComponentMask() });
		return entities.back().id;
	}*/

	std::vector<std::shared_ptr<ActorEntry>> freeActors;
	std::vector<ComponentPool> componentPools;
	std::vector<std::shared_ptr<ActorEntry>> actors;

	size_t maxActors = 1;

private:

	friend class MapParser;
	friend class Player;
	friend class PlayerLogic;

	//void init();

	//tilemap
	//std::vector<std::shared_ptr<Actor>> actors;
	Tilemap tilemap;
	Camera camera;
	Parallax parallaxEngine;
	PhysicsEngine engine;

	Window window;
	KeyboardInput input;
	//MapParser parser;

	//bool changeLevelFlag = false;
	//bool resetLevelFlag = false;

	//int currentLevel = 1;

	//std::string levelBaseName = "newLevel";
};

extern int componentCounter;
template <class T>
int getComponentId()
{
	static int componentId = componentCounter++;
	return componentId;
}


