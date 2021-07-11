#include "stdafx.h"
#include "Texture.h"

Texture::Texture(const std::string& filename) : filename(filename) {

	//initialize the sf::Texture file
	tex = std::make_unique<sf::Texture>();

	//set up the file path of the texture
	fs::path filepath(fs::current_path());
	filepath /= "resources";
	filepath /= "textures";
	filepath /= filename;
	filepath += ".png";

	if (!tex->loadFromFile(filepath.string())) {
#ifdef _DEBUG
		throw std::exception();
#endif
	}
}

sf::Texture& Texture::getTexture() {
	return *tex;
}

const sf::Texture& Texture::getTexture() const {
	return *tex;
}

std::string Texture::getName() const { 
	return filename; 
}