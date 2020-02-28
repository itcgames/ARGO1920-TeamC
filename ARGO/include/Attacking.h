#pragma once
#include "State.h"

class Attacking : public State
{
public:
	virtual void update(float t_dt) {}
	virtual SDL_Texture* getTexture() { return nullptr; }
	virtual SDL_Rect* getFrame() { return nullptr; }

	void idle(FiniteStateMachine* a);
	void moving(FiniteStateMachine* a);
	void attacking(FiniteStateMachine* a);
};

