#include "stdafx.h"
#include "FiniteStateMachine.h"
#include "Idle.h"

FiniteStateMachine::FiniteStateMachine() :
	m_current(new Idle())
{
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

void FiniteStateMachine::update(float t_dt)
{
	m_current->update(t_dt);

	if (m_moved)
	{
		moving();
	}
	else
	{
		idle();
	}
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
