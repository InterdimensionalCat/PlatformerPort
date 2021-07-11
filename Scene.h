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
#include "SceneEvent.h"

//class Tilemap;
//class Parallax;
//class Camera;
//class KeyboardInput;
//class PhysicsEngine;
//class AudioEngine;

class Actor;

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
			bool valid = scene->actors.at(index)->isIdValid() && scene->actors.at(index)->isActive();
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
		if (all) return Iterator(scene, 0, viewComponents, all);
		int index = 0;
		while (index < scene->actors.size()) {

			if (!scene->actors.at(index)->isIdValid() || !scene->actors.at(index)->isActive()) continue;

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


class Scene {
public:

	Scene();
	~Scene();


	void loadActors();

	void update();
	void draw(const float interpol);

	std::shared_ptr<AudioEngine> audio;

	template<typename T, typename... Args>
	T* assignComponent(std::shared_ptr<ActorEntry> entry, Args&&... args) {
		auto idind = entry->getIndex();
		int componentId = getComponentId<T>();
		if (componentPools.size() <= componentId) {
			componentPools.resize(componentId + 1, std::shared_ptr<TypedComponentPool<T>>(nullptr));
		}

		if (componentPools.at(componentId).get() == nullptr) {
			componentPools.at(componentId) = std::make_shared<TypedComponentPool<T>>(maxActors);
		}

		auto poolTyped = static_cast<TypedComponentPool<T>*>(componentPools.at(componentId).get());


		poolTyped->setIndex(idind, args... );
		T* component = static_cast<T*>(poolTyped->at(idind));
		//component->addJsonGenerator();

		//T* component = new (poolTyped->at(idind)) T{ args... };
		entry->setComponent(componentId);
		return component;
	}

	template<typename T>
	T* getComponent(const std::shared_ptr<ActorEntry> entry) {
		int componentId = getComponentId<T>();
		if (!entry->hasComponent(componentId)) {
			return nullptr;
		}

		T* component = static_cast<T*>(componentPools.at(componentId)->at(entry->getIndex()));
		return component;
	}

	template<typename T>
	const T* getComponent(const std::shared_ptr<ActorEntry> entry) const {
		int componentId = getComponentId<T>();
		if (!entry->hasComponent(componentId)) {
			return nullptr;
		}

		T* component = static_cast<T*>(componentPools.at(componentId).at(entry->getIndex()));
		return component;
	}

	template<typename T>
	void removeComponent(std::shared_ptr<ActorEntry> entry) {
		if (entry->isIdValid()) {
			int componentId = getComponentId<T>();
			entry->removeComponent(componentId);
			componentPools.at(componentId)->removeComponent(entry->getIndex());
		}
	}

	void despawnActor(std::shared_ptr<ActorEntry> entry);

	void respawnActor(std::shared_ptr<ActorEntry> entry);

	std::shared_ptr<json> createJsonFromActor(std::shared_ptr<ActorEntry> entry);

	std::shared_ptr<ActorEntry> spawnActor(const std::string& typeName, const std::string& variantName);
	std::shared_ptr<ActorEntry> spawnActor(const std::string& typeName, const std::string& variantName, const tmx::Object& obj);

	std::vector<std::shared_ptr<ActorEntry>> freeActors;
	std::vector<std::shared_ptr<ComponentPool>> componentPools;
	std::vector<std::shared_ptr<ActorEntry>> actors;

	size_t maxActors = 1;

	float getUpperBound() {
		return -1.0f;
	}

	float getLowerBound() {
		return mapheight + 1.0f;
	}

	float getLeftBound() {
		return 0.0f;
	}

	float getRightBound() {
		return mapwidth;
	}

	void saveActorJsonToFile(std::shared_ptr<json> data, const std::string& filename);

	template<typename T, typename... Args>
	void pushEvent(const Args&&... args) {
		events.push(std::make_unique<T>(std::forward<Args>(args)...));
	}


	std::shared_ptr<Tilemap> tilemap;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Parallax> parallaxEngine;

	std::shared_ptr<Window> window;
	std::shared_ptr<KeyboardInput> input;

	std::string currentlevelname;

	void loadLevel(const std::string& levelname);

	float mapwidth;
	float mapheight;
private:
	std::queue<std::unique_ptr<SceneEvent>> events;
};

extern int componentCounter;
template <class T>
int getComponentId()
{
	static int componentId = componentCounter++;
	return componentId;
}


