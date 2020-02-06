#pragma once
#include "Component.h"
#include "SDL.h"

class InputComponent : public Component
{
	struct Keys
	{
		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
	};

public:
	InputComponent();
	~InputComponent();
	void update();
	Keys m_keys;

private:
	const Uint8* m_keyStates = SDL_GetKeyboardState(NULL);
};

