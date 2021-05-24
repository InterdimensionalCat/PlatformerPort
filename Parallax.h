#pragma once
#include "Texture.h"

class Parallax
{
	class ParallaxLayer {
	public:
		ParallaxLayer(const std::string& texturename, const float modifierX, const float modifierY = 0) : tex(ic::Texture(texturename)), modifierX(modifierX), modifierY(modifierY) {
			tex.getTexture().setRepeated(true);
			sprite.setTexture(tex.getTexture());
		}
		void move(const float dx, const float dy = 0) {
			auto prevrect = sprite.getTextureRect();
			prevrect.left += toPixels(dx*modifierX);
			prevrect.top += toPixels(dy * modifierY);
			sprite.setTextureRect(prevrect);
		}
	private:
		ic::Texture tex;
		sf::Sprite sprite;
		float modifierX;
		float modifierY;
	};
};

