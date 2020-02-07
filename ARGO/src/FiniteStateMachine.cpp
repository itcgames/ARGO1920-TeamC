#include "stdafx.h"
#include "FiniteStateMachine.h"
#include "Idle.h"

FiniteStateMachine::FiniteStateMachine()
{
	m_current = new Idle();
}

FiniteStateMachine::~FiniteStateMachine()
{
}

void FiniteStateMachine::setCurrent(State* t_state)
{
	m_current = t_state;
}

void FiniteStateMachine::setPrevious(State* t_state)
{
	m_current = t_state;
}

State* FiniteStateMachine::getCurrent()
{
	return m_current;
}

State* FiniteStateMachine::getPrevious()
{
	return m_previous;
}

void FiniteStateMachine::update()
{
	m_current->update();
}

void FiniteStateMachine::idle()
{
	m_current->idle(this);
}

void FiniteStateMachine::moving()
{
	m_current->moving(this);
}

void FiniteStateMachine::attacking()
{
	m_current->attacking(this);
}
