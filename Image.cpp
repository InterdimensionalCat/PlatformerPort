#include "stdafx.h"
#include "Image.h"
#include "Texture.h"

using namespace ic;

Image::Image(const ic::Texture& tex) : image(std::make_shared<sf::Image>(tex.getTexture().copyToImage())) {

}

Image::Image(const std::string& filename) {
	//initialize the sf::Texture file
	image = std::make_shared<sf::Image>();

	//set up the file path of the texture
	fs::path filepath(fs::current_path());
	filepath /= "resources";
	filepath /= "images";
	filepath /= filename;
	filepath += ".png";

	if (!image->loadFromFile(filepath.string())) {
#ifdef _DEBUG
		throw std::exception();
#endif
	}
}

float Image::width() const {
	return (float)image->getSize().x;
}

float Image::height() const {
	return (float)image->getSize().y;
}

sf::Color Image::getColorAtPixel(uint32_t x, uint32_t y) const {
	return image->getPixel((unsigned int)x, (unsigned int)y);
}
