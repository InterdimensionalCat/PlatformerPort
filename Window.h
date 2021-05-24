#pragma once

namespace ic {
	class Window
	{
	public:

		Window() {

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

		~Window() {
			window->close();
		}

		void updateInput() {
			
			
				//poll an event
				sf::Event event;
				while (window->pollEvent(event))
				{
			
					if (event.type == sf::Event::Closed) {

					}
			
					if (event.type == sf::Event::KeyPressed) {

					}
			
					if (event.type == sf::Event::MouseButtonPressed) {

					}
				}
		}

		void preRender(const float interpol) {
			this->interpol = interpol;
			window->clear(sf::Color(255, 255, 255, 255));
		}


		void postRender() {
			window->display();
		}

		float interpol = 0.0f;
		sf::RenderStates states = sf::RenderStates::Default;
		std::unique_ptr<sf::RenderWindow> window;
	};
}

