#pragma once
#include "Texture.h"

namespace ic {
	class Window;
}

class Camera;
class LevelData;

class Parallax
{
public:
	Parallax(const LevelData& level);

	class ParallaxLayer {
	public:
		ParallaxLayer(const std::string& texture, const float repetitionPercentX, const float repetitionPercentY = 1.0);
		void move(Camera& camera, const float percentX, const float percentY);
		void draw(ic::Window& window) const;
	private:
		ic::Texture tex;
		sf::Sprite sprite;
		float repetitionPercentX;
		float repetitionPercentY;
	};

	void move(Camera& camera);

	void draw(ic::Window& window) const;
private:
	std::vector<ParallaxLayer> layers;
};

