#pragma once

class FiniteStateMachine
{
private:
	class State* m_current;
	class State* m_previous;
public:
	FiniteStateMachine();
	~FiniteStateMachine();
	void setCurrent(State* t_state);
	void setPrevious(State* t_state);
	State* getCurrent();
	State* getPrevious();

	void update();

	void idle();
	void moving();
	void attacking();
};

