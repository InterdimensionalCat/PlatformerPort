#pragma once
#include "Input.h"

class BoopAI :
    public Input
{
public:
	BoopAI();

	void update() override;
	bool isDown(const InputButton key) const override;
	bool isPressed(const InputButton key) const override;

	void pressKey(const InputButton key);
	void releaseKey(const InputButton key);

	_Input_Device(BoopAI);

private:

	KeyListener<InputButton> keylistener;
};

