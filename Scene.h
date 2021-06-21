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



class Scene {
public:

	template<typename... ComponentTypes>
	class SceneView {
	public:
		SceneView(Scene* scene) : scene(scene), all(sizeof...(ComponentTypes) == 0) {
			if (!all) {
				viewComponents = { 0, getComponentId<ComponentTypes>() ... };
			}
		}

		struct Iterator {
			Iterator(Scene* scene, size_t index, const std::vector<int>& viewComponents, bool all)
				: scene(scene), index(index), viewComponents(viewComponents), all(all) {}

			std::shared_ptr<ActorEntry> operator*() const {
				return scene->actors.at(index);
			}

			std::shared_ptr<ActorEntry> operator*() {
				return scene->actors.at(index);
			}

			bool operator==(const Iterator& other) const {
				return index == other.index || index == scene->actors.size();
			}
			bool operator!=(const Iterator& other) const {
				return index != other.index && index != scene->actors.size();
			}

			bool ValidIndex() {
				bool valid = scene->actors.at(index)->isIdValid();
				if (!valid) return valid;
				if (all && valid) return valid;
				for (size_t i = 0; i < viewComponents.size(); i++) {
					if (!scene->actors.at(index)->hasComponent(viewComponents.at(i))) {
						valid = false;
						break;
					}
				}
				return valid;
			}

			Iterator& operator++() {
				index++;
				while (index < scene->actors.size() && !ValidIndex()) {
					index++;
				}
				return *this;
			}


			Scene* scene;
			size_t index;
			std::vector<int> viewComponents;
			bool all;
		};

		const Iterator begin() const {
			if(all) return Iterator(scene, 0, viewComponents, all);
			int index = 0;
			while (index < scene->actors.size()) {

				if (!scene->actors.at(index)->isIdValid()) continue;

				bool validFirst = true;
				for (size_t i = 0; i < viewComponents.size(); i++) {
					if (!scene->actors.at(index)->hasComponent(viewComponents.at(i))) {
						validFirst = false;
						break;
					}
				}

				if (validFirst) {
					break;
				}
				else {
					index++;
				}
			}
			return Iterator(scene, index, viewComponents, all);
		}

		const Iterator end() const
		{
			return Iterator(scene, scene->actors.size(), viewComponents, all);
		}

		std::vector<int> viewComponents; 
		Scene* scene;
		bool all;
	};

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

		updateTransforms();

		for (auto& pool : componentPools) {
			pool.logData();
		}
	}
	~Scene() {}

	void updateTransforms() {
		for (auto actor : SceneView<Transform, Shape>(this)) {
			Transform* trans = getComponent<Transform>(actor);
			//trans->pos.x += 1;

			Logger::get() << "transform X: " << trans->pos.x << "\n";
		}
	}


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


