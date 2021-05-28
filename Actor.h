#pragma once
#include "Window.h"
#include "Component.h"

class Actor
{
public:
	Actor(const std::string& name,
		std::unique_ptr<RenderComponent> renderer,
		std::unique_ptr<PhysicsComponent> physics,
		std::unique_ptr<LogicComponent> logic)
		: name(name), renderer(std::move(renderer)),
		physics(std::move(physics)), logic(std::move(logic)) {

		this->renderer->setActor(this);
		this->physics->setActor(this);
		this->logic->setActor(this);
	}

	void update() {
		logic->update();
		physics->update();
		renderer->update();
	}

	void draw(ic::Window& window) const {
		renderer->draw(window);
	}


	std::unique_ptr<RenderComponent> renderer;
	std::unique_ptr<PhysicsComponent> physics;
	std::unique_ptr<LogicComponent> logic;
	std::string name;
};

