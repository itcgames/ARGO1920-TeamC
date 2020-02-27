#include "stdafx.h"
#include "PrimitiveComponent.h"

PrimitiveComponent::PrimitiveComponent(glm::vec2 t_size, glm::vec2 t_pos, bool t_staticPosition) :
	Component(ComponentType::Primitive),
	m_position(t_pos),
	m_size(t_size),
	m_staticPosition(t_staticPosition)
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
