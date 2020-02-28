#pragma once
#include "State.h"

class Idle : public State
{
public:
	virtual void update(float t_dt) {}
	virtual void getTexture() {}
	virtual void getFrame() {}

	void idle(FiniteStateMachine* a);
	void moving(FiniteStateMachine* a);
	void attacking(FiniteStateMachine* a);
};

