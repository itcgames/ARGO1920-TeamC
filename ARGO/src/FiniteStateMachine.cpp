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

void FiniteStateMachine::setCurrent(State* s)
{
	m_current = s;
}

void FiniteStateMachine::setPrevious(State* s)
{
	m_current = s;
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
