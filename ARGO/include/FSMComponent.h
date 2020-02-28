#pragma once
#include "Component.h"
#include "FiniteStateMachine.h"
class FSMComponent :
	public Component
{
public:
	FSMComponent();
	FiniteStateMachine& getFSM();

private:
	FiniteStateMachine m_fsm;
};

