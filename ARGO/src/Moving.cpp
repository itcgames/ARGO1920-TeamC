#include "stdafx.h"
#include "Moving.h"
#include "Idle.h"
#include "Attacking.h"

void Moving::idle(FiniteStateMachine* a)
{
	std::cout << "Moving -> Idle" << std::endl;
	a->setCurrent(new Idle());
	delete this;
}

void Moving::moving(FiniteStateMachine* a)
{
	std::cout << "Moving -> Moving" << std::endl;
	a->setCurrent(new Moving());
	delete this;
}

void Moving::attacking(FiniteStateMachine* a)
{
	std::cout << "Moving -> Attacking" << std::endl;
	a->setCurrent(new Attacking());
	delete this;
}
