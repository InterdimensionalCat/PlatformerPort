#pragma once
#include "Texture.h"
#include "Window.h"
#include "LevelData.h"
#include <vector>
#include "Camera.h"

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
		ParallaxLayer(const std::string& texture, const float repetitionPercentX, const float repetitionPercentY = 1.0) : 
			tex(ic::Texture(texture)), repetitionPercentX(repetitionPercentX), repetitionPercentY(repetitionPercentY) {
			tex.getTexture().setRepeated(true);
			sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(Settings::getSetting<int>("Width"), Settings::getSetting<int>("Height"))));
			sprite.setTexture(tex.getTexture());
			//repetitionPercentX = tex.getTexture().getSize().x / toPixels(data.levelWidth);
		}
		void move(Camera& camera, const float percentX, const float percentY) {
			auto prevrect = sprite.getTextureRect();
			prevrect.left = (int)(percentX*(repetitionPercentX*(float)tex.getTexture().getSize().x - (float)Settings::getSetting<int>("Width")));
			prevrect.top  = (int)(percentY*(repetitionPercentY*(float)tex.getTexture().getSize().y - (float)Settings::getSetting<int>("Height")));


			sprite.setTextureRect(prevrect);
			sprite.setPosition(sf::Vector2f(0, 0));
			sprite.move(camera.scrollpos);
		}
		void draw(ic::Window& window) const {
			window.window->draw(sprite, window.states);
		}
	private:
		ic::Texture tex;
		sf::Sprite sprite;
		float repetitionPercentX;
		float repetitionPercentY;
	};

	void move(Camera& camera) {

		//calc scroll here xd
		auto percentX = camera.scrollpos.x / (camera.boundsInPixels.x - (float)Settings::getSetting<int>("Width"));
		auto percentY = camera.scrollpos.y / (camera.boundsInPixels.y - (float)Settings::getSetting<int>("Height"));

		//if (percentX != 0.0f || percentY != 0.0f) {
			for (auto& layer : layers) {
				layer.move(camera, percentX, percentY);
			}
		//}
	}

	void draw(ic::Window& window) const {
		for (auto& layer : layers) {
			layer.draw(window);
		}
	}
private:
	std::vector<ParallaxLayer> layers;
};

