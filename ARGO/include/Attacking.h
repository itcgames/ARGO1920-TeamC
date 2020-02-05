#pragma once
#include "State.h"

class Attacking : public State
{
public:
	void update() {  }

	void idle(FiniteStateMachine* a);
	void moving(FiniteStateMachine* a);
	void attacking(FiniteStateMachine* a);
};

