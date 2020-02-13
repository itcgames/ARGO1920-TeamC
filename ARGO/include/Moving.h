#pragma once
#include "State.h"
#include "Idle.h"

class Moving : public State
{
public:
	void update() {  }

	void idle(FiniteStateMachine* a);
	void moving(FiniteStateMachine* a);
	void attacking(FiniteStateMachine* a);
};

