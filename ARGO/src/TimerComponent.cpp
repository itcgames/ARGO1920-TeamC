#include "stdafx.h"
#include "TimerComponent.h"

TimerComponent::TimerComponent(float t_maxTime) :
	Component(ComponentType::Timer),
	m_maxTime(t_maxTime),
	m_currentTime(t_maxTime)
{
}

bool TimerComponent::tick(float t_time)
{
	if (m_currentTime > 0)
	{
		m_currentTime -= t_time;
		return true;
	}
	return false;
}

void TimerComponent::setMaxTime(float t_time)
{
	m_maxTime = t_time;
}

float TimerComponent::getCurrentTime()
{
	return m_currentTime;
}

bool TimerComponent::finished()
{
	return m_currentTime <= 0;
}

void TimerComponent::reset()
{
	m_currentTime = m_maxTime;
}
