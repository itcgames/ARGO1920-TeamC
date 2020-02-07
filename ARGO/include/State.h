#pragma once
#include "FiniteStateMachine.h"

class State
{
public:
	virtual void update() {}

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

