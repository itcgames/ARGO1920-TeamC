#include "stdafx.h"
#include "PrimitiveComponent.h"

PrimitiveComponent::PrimitiveComponent(glm::vec2 t_size, glm::vec2 t_pos, bool t_staticPosition,float t_minimumSize, float t_maximumSize) :
	Component(ComponentType::Primitive),
	m_position(t_pos),
	m_size(t_size),
	m_staticPosition(t_staticPosition),
	m_maximumSize(t_maximumSize),
	m_minimumSize(t_minimumSize)
{
}

PrimitiveComponent::PrimitiveComponent():
Component(ComponentType::Primitive)
{
}

PrimitiveComponent::~PrimitiveComponent()
{
}

void PrimitiveComponent::setSize(glm::vec2 t_size)
{
	m_size = t_size;
}

glm::vec2 PrimitiveComponent::getSize()
{
	return m_size;
}

void PrimitiveComponent::setPosition(glm::vec2 t_pos)
{
	m_position = t_pos;
}

glm::vec2 PrimitiveComponent::getPosition()
{
	return m_position;
}

void PrimitiveComponent::setStaticPosition(bool t_staticPosition)
{
	m_staticPosition = t_staticPosition;
}

bool PrimitiveComponent::getStaticPosition()
{
	return m_staticPosition;
}

float PrimitiveComponent::getMinimumSize()
{
	return m_minimumSize;
}

void PrimitiveComponent::setMinimumSize(float t_minimum)
{
	m_minimumSize = t_minimum;
}

float PrimitiveComponent::getMaximumSize()
{
	return m_maximumSize;
}

void PrimitiveComponent::setMaximumSize(float t_maximum)
{
	m_maximumSize = t_maximum;
}
