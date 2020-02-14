#pragma once
#include "Component.h"
class TimerComponent : public Component
{
public:
	TimerComponent(float t_maxTime);
	bool tick(float t_time);
	void setMaxTime(float t_time);
	float getCurrentTime();
	bool finished();
	void reset();
private:
	float m_currentTime;
	float m_maxTime;
};
