#include "stdafx.h"
#include "ForceComponent.h"


ForceComponent::ForceComponent(glm::vec2 t_force, bool t_hasFriction) :
	Component(ComponentType::Force),
	m_force(t_force),
	m_hasFriction(t_hasFriction)
{
}

ForceComponent::~ForceComponent()
{
}

glm::vec2 ForceComponent::getForce() const
{
	return m_force;
}

float ForceComponent::getForceAngle() const
{
	return std::atan2(m_force.y, m_force.x);
}

void ForceComponent::setForce(float t_x, float t_y)
{
	m_force.x = t_x;
	m_force.y = t_y;
}

void ForceComponent::setForce(glm::vec2 t_force)
{
	m_force = t_force;
}

void ForceComponent::addForce(glm::vec2 t_force)
{
	m_force += t_force;
}

void ForceComponent::addForce(float t_x, float t_y)
{
	m_force += glm::vec2(t_x, t_y);
}

void ForceComponent::addForceX(float t_x)
{
	m_force.x += t_x;
}

void ForceComponent::addForceY(float t_y)
{
	m_force.y += t_y;
}

void ForceComponent::setForceDirection(float t_angleDeg)
{
	float sin = std::sin(glm::radians(t_angleDeg));
	float cos = std::cos(glm::radians(t_angleDeg));

	m_force = glm::vec2(m_force.length() * cos, m_force.length() * sin);
}

float ForceComponent::getMag() const
{
	return m_force.length();
}

glm::vec2 ForceComponent::getUnit()
{
	return glm::normalize(m_force);
}

void ForceComponent::setX(float t_x)
{
	m_force.x = t_x;
}

void ForceComponent::setY(float t_y)
{
	m_force.y = t_y;
}

void ForceComponent::setHasFriction(bool t_hasFriction)
{
	m_hasFriction = t_hasFriction;
}

bool ForceComponent::getHasFriction()
{
	return m_hasFriction;
}
