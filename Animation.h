#pragma once
#pragma once

#include "Texture.h"


class Window;

//Animation.h
/*
  Object that populates the Animator map; contains small animations loaded from a spritemap in a left-right up-down sequence;
  EX.png:
   ________________________
  | frame1 2       3     4 |
  | 5      6       7     8 |
  | etc...                 |
  |________________________|
  Animation length is limited by type of graphics card. I dont know how to get max texture size supported for a graphics card,
  but for sure anything larger than 8192 in either direction is too large, this may eventually limit the duration of animations
  but with the above restriction almost 9 seconds of fairly high resolution sprite animation (500x500px) which is quite a long
  time for a single animation, and if it ever becomes a problem I can probably rewrite it so that animations can be daisy chained
  together like a linked list
*/

class Animation
{
public:
	/*
	Params:
	frameDim: dimension of each frame on the texture
	targetDim: dimension of each frame to be output in the window
	numFrames: the number of frames in the animation
	offset: a small visual translation that moves
	this animation away from the center whenever this frame is drawn
	*/
	Animation(const std::string& filename, const sf::Vector2i& frameDim, const sf::Vector2f& targetDim, const int firstFrame, const int numFrames, const int fps);


	//top left corner
	void setPositition(const sf::Vector2f& posInPixels);

	void draw(Window& window);
	void advanceFrame();

	std::string getName() const { return name; };
	void setFlipped(const bool flip);
	void reset();


	sf::Sprite frame;

	Texture tex;
	int framerate;
	int frameNum;
	int numFrames;
	int firstFrame;
	int framesPerRow;
	std::string name;
	sf::Vector2i frameDim;
	sf::Vector2f targetDim;

	int framesTillNext;
	int activeFrames;
	bool flip = false;
};

