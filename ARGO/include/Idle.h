#pragma once
#include "State.h"

class Idle : public State
{
public:
	Idle() 
	{
		m_texture = AssetManager::Instance()->GetTexture("idle.png");
		m_frame.x = 0;
		m_frame.y = 0;
		m_frame.h = 40;
		m_frame.w = 42;
	};
	virtual void update(float t_dt) 
	{
	}
	virtual SDL_Texture* getTexture() { return m_texture; }
	virtual SDL_Rect* getFrame() 
	{ 
		return &m_frame; 
	}

	void idle(FiniteStateMachine* a);
	void moving(FiniteStateMachine* a);
	void attacking(FiniteStateMachine* a);

private:
	SDL_Texture* m_texture;
	SDL_Rect m_frame;
};

