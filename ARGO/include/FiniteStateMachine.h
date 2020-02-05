#pragma once

class FiniteStateMachine
{
private:
	class State* m_current;
	class State* m_previous;
public:
	FiniteStateMachine();
	~FiniteStateMachine();
	void setCurrent(State* s);
	void setPrevious(State* s);
	State* getCurrent();
	State* getPrevious();

	void update();

	void idle();
	void moving();
	void attacking();
};

