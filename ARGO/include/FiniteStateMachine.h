#pragma once

class FiniteStateMachine
{
private:
	class State* m_current;
	class State* m_previous;

	bool m_moved;
public:
	FiniteStateMachine();
	~FiniteStateMachine();
	void setCurrent(State* t_state);
	void setPrevious(State* t_state);
	State* getCurrent();
	State* getPrevious();
	void setMoved(bool t_value) { m_moved = t_value; };

	void update(float t_dt);

	void idle();
	void moving();
	void attacking();
};

