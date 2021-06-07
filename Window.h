#pragma once

namespace ic {
	class Window
	{
	public:

		Window();

		~Window();

		void updateInput();

		void preRender(const float interpol);


		void postRender();

		float interpol = 0.0f;
		sf::RenderStates states = sf::RenderStates::Default;
		std::unique_ptr<sf::RenderWindow> window;
	};
}

