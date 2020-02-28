#include "stdafx.h"
#include "FSMComponent.h"

FSMComponent::FSMComponent() :
	Component(ComponentType::FSM)
{
}

FiniteStateMachine& FSMComponent::getFSM()
{
	return m_fsm;
}
