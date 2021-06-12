#pragma once

class WindowEventListener;

class Window
{
public:

	Window();

	Window(const Window& other) = delete;
	Window& operator=(const Window& rhs) = delete;

	~Window();

	bool updateInput();

	void preRender(const float interpol);


	void postRender();

	void registerWindowEventListener(std::shared_ptr<WindowEventListener> listener);

	float interpol = 0.0f;
	sf::RenderStates states = sf::RenderStates::Default;
	std::unique_ptr<sf::RenderWindow> window;

private:
	std::vector< std::shared_ptr<WindowEventListener>> listeners;
};

