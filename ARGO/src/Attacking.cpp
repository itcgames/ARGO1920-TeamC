#include "stdafx.h"
#include "Attacking.h"
#include "Moving.h"
#include "Idle.h"

void Attacking::idle(FiniteStateMachine* a)
{
	std::cout << "Attacking -> Idle" << std::endl;
	a->setCurrent(new Idle());
	delete this;
}

void Attacking::moving(FiniteStateMachine* a)
{
	std::cout << "Attacking -> Moving" << std::endl;
	a->setCurrent(new Moving());
	delete this;
}

void Attacking::attacking(FiniteStateMachine* a)
{
	std::cout << "Attacking -> Attacking" << std::endl;
	a->setCurrent(new Attacking());
	delete this;
}
