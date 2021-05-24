#pragma once

namespace ic {

	class Texture;

	class Image
	{
	public:
		Image(const ic::Texture& tex);
		Image(const std::string& filename);
		float width() const;
		float height() const;
		sf::Color getColorAtPixel(uint32_t x, uint32_t y) const;
	private:
		std::shared_ptr<sf::Image> image;
	};
}


