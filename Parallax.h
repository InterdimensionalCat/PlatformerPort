#pragma once
#include "Texture.h"
#include "Window.h"
#include "LevelData.h"
#include <vector>

class Parallax
{
public:
	Parallax(const LevelData& level) {
		int bgCounter = 1;
		fs::path filepath(fs::current_path());
		filepath /= "resources";
		filepath /= "textures";

		while (fs::exists(filepath / (level.parallaxTextureName + "_" + std::to_string(bgCounter) + ".png"))) {
			layers.push_back(ParallaxLayer(
				level.parallaxTextureName + "_" + std::to_string(bgCounter)
				, level.baseScrollPercent + (float)bgCounter * level.scrollGrowth));
			bgCounter++;
		}
	}

	class ParallaxLayer {
	public:
		ParallaxLayer(const std::string& texture, const float repititonPercentX, const float repititionPercentY = 1.0) : 
			tex(ic::Texture(texture)) {
			tex.getTexture().setRepeated(true);
			sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(Settings::getSetting<int>("Width"), Settings::getSetting<int>("Height"))));
			sprite.setTexture(tex.getTexture());
			//repititionPercentX = tex.getTexture().getSize().x / toPixels(data.levelWidth);
		}
		void move(const float percentX, const float percentY) {
			auto prevrect = sprite.getTextureRect();
			prevrect.left +=(int)toPixels(percentX*repititionPercentX*(float)tex.getTexture().getSize().x);
			prevrect.top += (int)toPixels(percentY*repititionPercentY*(float)tex.getTexture().getSize().y);
			sprite.setTextureRect(prevrect);
		}
		void draw(ic::Window& window) const {
			window.window->draw(sprite, window.states);
		}
	private:
		ic::Texture tex;
		sf::Sprite sprite;
		float repititionPercentX;
		float repititionPercentY;
	};

	void move() {

		//calc scroll here xd

		for (auto& layer : layers) {
			layer.move(0,0);
		}
	}

	void draw(ic::Window& window) const {
		for (auto& layer : layers) {
			layer.draw(window);
		}
	}
private:
	std::vector<ParallaxLayer> layers;
};

