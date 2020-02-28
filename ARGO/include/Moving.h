#pragma once
#include "State.h"
#include "Idle.h"

class Moving : public State
{
public:
	Moving()
	{
		m_texture = AssetManager::Instance()->GetTexture("legAnim.png");
		m_currentFrame = 0;
		m_frameDelay = 3;
		m_maxFrames = 7;
		m_goingForward = true;
		m_frameTimer = 0;
		m_frame.x = 0;
		m_frame.y = 0;
		m_frame.h = 40;
		m_frame.w = 42;
	};
	virtual void update(float t_dt) 
	{
		m_frameTimer += t_dt;
		if (m_frameTimer >= m_frameDelay)
		{
			m_currentFrame += (m_goingForward ? 1 : -1);
			m_frameTimer = 0.0f;
		}
		if (m_currentFrame > m_maxFrames)
		{
			m_currentFrame = m_maxFrames - 1;
			m_goingForward = false;
		}
		if (m_currentFrame < 0)
		{
			m_currentFrame = 1;
			m_goingForward = true;
		}
	}
	virtual SDL_Texture* getTexture() { return m_texture; }
	virtual SDL_Rect* getFrame() 
	{
		m_frame.x = 42 * m_currentFrame;
		return &m_frame; 
	}

	void idle(FiniteStateMachine* a);
	void moving(FiniteStateMachine* a);
	void attacking(FiniteStateMachine* a);

private:
	int m_currentFrame;
	int m_maxFrames;
	bool m_goingForward;
	float m_frameTimer;
	float m_frameDelay;
	SDL_Texture* m_texture;
	SDL_Rect m_frame;
};

