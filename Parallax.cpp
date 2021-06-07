#include "stdafx.h"
#include "Parallax.h"
#include "Window.h"
#include "LevelData.h"
#include "Camera.h"


Parallax::Parallax(const LevelData& level) {
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

Parallax::ParallaxLayer::ParallaxLayer(const std::string& texture, const float repetitionPercentX, const float repetitionPercentY) :
	tex(ic::Texture(texture)), repetitionPercentX(repetitionPercentX), repetitionPercentY(repetitionPercentY) {
	tex.getTexture().setRepeated(true);
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(Settings::getSetting<int>("Width"), Settings::getSetting<int>("Height"))));
	sprite.setTexture(tex.getTexture());
	//repetitionPercentX = tex.getTexture().getSize().x / toPixels(data.levelWidth);
}
void Parallax::ParallaxLayer::move(Camera& camera, const float percentX, const float percentY) {
	auto prevrect = sprite.getTextureRect();
	if (repetitionPercentY >= 1.0f) {
		prevrect.left = (int)(percentX * (repetitionPercentX * (float)tex.getTexture().getSize().x - (float)Settings::getSetting<int>("Width")));
	}
	else {
		prevrect.left = (int)(percentX * (repetitionPercentX * (float)tex.getTexture().getSize().x - (float)Settings::getSetting<int>("Width") * 0.0f));
	}

	if (repetitionPercentY >= 1.0f) {
		prevrect.top = (int)(percentY * (repetitionPercentY * (float)tex.getTexture().getSize().y - (float)Settings::getSetting<int>("Height")));
	}
	else {
		prevrect.top = (int)(percentY * (repetitionPercentY * (float)tex.getTexture().getSize().y - (float)Settings::getSetting<int>("Height") * 0.0f));
	}




	sprite.setTextureRect(prevrect);
	sprite.setPosition(sf::Vector2f(0, 0));
	sprite.move(camera.scrollpos);
}
void Parallax::ParallaxLayer::draw(ic::Window& window) const {
	window.window->draw(sprite, window.states);
}

void Parallax::move(Camera& camera) {

	//calc scroll here xd

	float percentX = 0;
	float percentY = 0;

	if (camera.boundsInPixels.x != (float)Settings::getSetting<int>("Width")) {
		percentX = camera.scrollpos.x / (camera.boundsInPixels.x - (float)Settings::getSetting<int>("Width"));
	}

	if (camera.boundsInPixels.y != (float)Settings::getSetting<int>("Height")) {
		percentY = camera.scrollpos.y / (camera.boundsInPixels.y - (float)Settings::getSetting<int>("Height"));
	}

	//if (percentX != 0.0f || percentY != 0.0f) {
	for (auto& layer : layers) {
		layer.move(camera, percentX, percentY);
	}
	//}
}

void Parallax::draw(ic::Window& window) const {
	for (auto& layer : layers) {
		layer.draw(window);
	}
}