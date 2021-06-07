#include "stdafx.h"
#include "Animation.h"
#include "Window.h"

using namespace sf;

//params: fdim = dimension of 1 frame on the texture, tdim = dimension 1 frame to output
Animation::Animation(const std::string& filename, const sf::Vector2i& frameDim, const sf::Vector2f& targetDim, const int firstFrame, const int numFrames, const int fps) : tex(ic::Texture(filename)), 
	frameDim(frameDim), targetDim(targetDim), firstFrame(firstFrame), numFrames(numFrames), framerate(fps) {

	tex.getTexture().setRepeated(true);

	frame.setTexture(tex.getTexture());
	frame.setTextureRect(IntRect(0, 0, frameDim.x, frameDim.y));
	frame.setScale(Vector2f(this->targetDim.x / (float)frameDim.x, this->targetDim.y / (float)frameDim.y));

	frameNum = firstFrame;

#ifdef _DEBUG
	auto test = (int)tex.getTexture().getSize().x / frameDim.x * (int)tex.getTexture().getSize().y / frameDim.y;
	assert(numFrames <= test);
#endif
	framesPerRow = tex.getTexture().getSize().x / frameDim.x;
	activeFrames = (int)Settings::getSetting<float>("targetFPS") / framerate;
	framesTillNext = activeFrames;

}

void Animation::setPositition(const sf::Vector2f& posInPixels) {
	frame.setPosition(round(posInPixels.x), round(posInPixels.y));
}

void Animation::setFlipped(bool flip) {

	Animation::flip = flip;

	if (flip) {
		Vector2i frameTopLeft(((frameNum + 1) % framesPerRow) * frameDim.x, (frameNum / framesPerRow) * frameDim.y);
		frame.setTextureRect(IntRect(frameTopLeft, Vector2i(-frameDim.x, frameDim.y)));

		//Logger::get() << "Texture Rect: (" << frameTopLeft.x << ", " << frameTopLeft.y << ", " << -frameDim.x << ", " << frameDim.y << ")\n";

	}
	else {
		Vector2i frameTopLeft((frameNum % framesPerRow) * frameDim.x, (frameNum / framesPerRow) * frameDim.y);
		frame.setTextureRect(IntRect(frameTopLeft, Vector2i(frameDim.x, frameDim.y)));
	}
}

void Animation::reset() {
	frameNum = firstFrame;
	framesTillNext = 0;
	setFlipped(false);
}


void Animation::draw(ic::Window& window) {

	auto states = window.states;
	//auto target = renderer->window.get();

	if (--framesTillNext <= 0) {
		advanceFrame();
		framesTillNext = activeFrames;
	}

	//interpolate frame
	//frame.setOrigin(lerp(parent->transform->getPrevPos().toSFMLVec<float>() - targetDim / 2.0f, 
	//	parent->transform->getPos().toSFMLVec<float>() - targetDim / 2.0f, renderer->interpol));

	//frame.setPosition(glerp(parent->transform->getPrevPos().toSFMLVec<float>() - targetDim / 2.0f + offset,
	//	parent->transform->getPos().toSFMLVec<float>() - targetDim / 2.0f + offset, renderer->interpol));

	window.window->draw(frame, states);

}

void Animation::advanceFrame() {
	//frameNum = (frameNum + 1) % numFrames;
	frameNum = (frameNum + 1);
	if (frameNum == (numFrames + firstFrame)) {
		frameNum = firstFrame;
	}

	if (flip) {
		Vector2i frameTopLeft(((frameNum + 1) % framesPerRow) * frameDim.x, (frameNum / framesPerRow) * frameDim.y);
		frame.setTextureRect(IntRect(frameTopLeft, Vector2i(-frameDim.x, frameDim.y)));

		//Logger::get() << "Texture Rect: (" << frameTopLeft.x << ", " << frameTopLeft.y << ", " << -frameDim.x << ", " << frameDim.y << ")\n";

	}
	else {
		Vector2i frameTopLeft((frameNum % framesPerRow) * frameDim.x, (frameNum / framesPerRow) * frameDim.y);
		frame.setTextureRect(IntRect(frameTopLeft, Vector2i(frameDim.x, frameDim.y)));
	}
}
