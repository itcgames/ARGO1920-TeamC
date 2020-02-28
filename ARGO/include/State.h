#pragma once
#include "FiniteStateMachine.h"
#include "AssetManager.h"

struct stateData
{
	int currentFrame;
	int totalFrames;
	float frameTimer; 
	float timePerFrame;
	SDL_Texture* texture;
};

class State
{
public:
	virtual void update(float t_dt) {}
	virtual SDL_Texture* getTexture() { return nullptr; }
	virtual SDL_Rect* getFrame() { return nullptr; }

	virtual void idle(FiniteStateMachine* a)
	{
		std::cout << "no transition to idle" << std::endl;
	}
	virtual void moving(FiniteStateMachine* a)
	{
		std::cout << "no transition to moving" << std::endl;
	}
	virtual void attacking(FiniteStateMachine* a)
	{
		std::cout << "no transition to attacking" << std::endl;
	}

};

