#pragma once
#include "Scene.h"
#include "Components.h"

template<typename... ComponentTypes>
class System {
public:
	System(Scene* scene) : scene(scene), view(SceneView<ComponentTypes...>(scene)) {}

	void excecute() {
		for (auto actor : view) {
			excecutionFunction(actor);
		}
	}
protected:
	virtual void excecutionFunction(std::shared_ptr<ActorEntry> entry) = 0;

	Scene* scene;
	SceneView<ComponentTypes...> view;
};

