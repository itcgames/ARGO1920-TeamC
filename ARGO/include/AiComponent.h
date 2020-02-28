#pragma once
#include "Component.h"
#include "AIStates.h"

class AiComponent : public Component
{
public:
	AiComponent(AITypes t_types, AIStates t_states, float t_maxRotation, float t_maxSpeed);
	AiComponent();
	~AiComponent();

	void setState(AIStates t_state);
	void setType(AITypes t_type);
	AITypes getType();
	AIStates getStates();
	void setMaxRotation(float t_maxRotation);
	float getMaxRotation();
	void setMaxSpeed(float t_maxSpeed);
	float getMaxSpeed();
	bool getIsleader();
	void setIsLeaser(bool t_value);

private:
	AITypes m_type = AITypes::eMelee;
	AIStates m_state = AIStates::eWander;
	float m_maxRotation = 2.0f; //3 degrees either side shift per frame. Used In AI
	float m_maxSpeed = 5.0f;//max speed Per Frame. Used By AI.
	bool m_isLeader;
};

