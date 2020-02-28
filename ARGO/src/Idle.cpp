#include "stdafx.h"
#include "Idle.h"
#include "Moving.h"
#include "Attacking.h"

void Idle::idle(FiniteStateMachine* a)
{
}

void Idle::moving(FiniteStateMachine* a)
{
	std::cout << "Idle -> Moving" << std::endl;
	a->setCurrent(new Moving());
	delete this;
}

void Idle::attacking(FiniteStateMachine* a)
{
	std::cout << "Idle -> Attacking" << std::endl;
	a->setCurrent(new Attacking());
	delete this;
}
