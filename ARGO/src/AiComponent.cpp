#include "stdafx.h"
#include "AiComponent.h"

AiComponent::AiComponent(AITypes t_types, AIStates t_states, float t_maxRot, float t_maxSpeed):
	Component(ComponentType::Ai),
	m_maxRotation(t_maxRot),
	m_maxSpeed(t_maxSpeed),
	m_type(t_types),
	m_state(t_states)
{
}

AiComponent::AiComponent() :
	Component(ComponentType::Ai)
{
}

AiComponent::~AiComponent()
{
}

void AiComponent::setState(AIStates t_state)
{
	m_state = t_state;
}

void AiComponent::setType(AITypes t_type)
{
	m_type = t_type;
}

AITypes AiComponent::getType()
{
	return m_type;
}

AIStates AiComponent::getStates()
{
	return m_state;
}
void AiComponent::setMaxRotation(float t_maxRot)
{
	m_maxRotation = t_maxRot;
}

float AiComponent::getMaxRotation()
{
	return m_maxRotation;
}

void AiComponent::setMaxSpeed(float t_maxSpeed)
{
	m_maxSpeed = t_maxSpeed;
}

float AiComponent::getMaxSpeed()
{
	return m_maxSpeed;
}