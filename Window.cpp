#include "stdafx.h"
#include "Window.h"
#include "WindowEventListener.h"

Window::Window() {

	//creates an SFML window using the width, height, and name specified in the game Settings
	window = std::make_unique<sf::RenderWindow>(
		sf::VideoMode(Settings::getSetting<int>("Width"), Settings::getSetting<int>("Height")),
		Settings::getSetting<std::string>("Title")
		);

	if (Settings::getSetting<bool>("vsync")) {
		window->setVerticalSyncEnabled(true);
	}
	else {
		window->setFramerateLimit((unsigned int)Settings::getSetting<float>("targetFPS"));
	}

	//causes keypresses to only send one window event
	window->setKeyRepeatEnabled(false);
}

Window::~Window() {
	listeners.clear();
	window->close();
}

bool Window::updateInput() {


	//poll an event
	sf::Event event;
	while (window->pollEvent(event))
	{

		for (auto& listener : listeners) {
			if (listener->handleWindowEvent(event)) {
				continue;
			}
		}

		if (event.type == sf::Event::Closed) {
			//Settings::setSetting<bool>("running", false);
			return false;
		}

		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape) {
				//Settings::setSetting<bool>("running", false);
				return false;
			}
		}

		if (event.key.code == sf::Keyboard::I) {
			Settings::setSetting<bool>("debug", !Settings::getSetting<bool>("debug"));
		}
	}

	return true;
}

void Window::preRender(const float interpol) {
	this->interpol = interpol;
	window->clear(sf::Color(255, 255, 255, 255));
}


void Window::postRender() {
	window->display();
}

void Window::registerWindowEventListener(std::shared_ptr<WindowEventListener> listener) {
	listeners.push_back(listener);
}
