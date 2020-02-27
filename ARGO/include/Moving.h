#pragma once
#include "State.h"
#include "Idle.h"

class Moving : public State
{
public:
	virtual void update(float t_dt) {}
	virtual void getTexture() {}
	virtual void getFrame() {}

	void idle(FiniteStateMachine* a);
	void moving(FiniteStateMachine* a);
	void attacking(FiniteStateMachine* a);
};

