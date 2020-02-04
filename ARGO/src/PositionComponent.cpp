#include "stdafx.h"
#include "PositionComponent.h"
#include <random>

PositionComponent::PositionComponent(float t_x, float t_y) :
	Component(ComponentType::Position)
{
	m_pos.x = t_x;
	m_pos.y = t_y;
}

PositionComponent::PositionComponent() :
	Component(ComponentType::Position)
{
	m_pos.x = 50 + std::rand() % ((750 + 1) - 50);
	m_pos.y = 50 + std::rand() % ((550 + 1) - 50);
}

PositionComponent::~PositionComponent()
{
}

PositionComponent::Position PositionComponent::getPos()
{
	return m_pos;
}

void PositionComponent::setPos(float t_x, float t_y)
{
	m_pos.x = t_x;
	m_pos.y = t_y;
}

void PositionComponent::setX(float t_x)
{
	m_pos.x = t_x;
}

void PositionComponent::setY(float t_y)
{
	m_pos.y = t_y;
}

void PositionComponent::moveLeft()
{
	m_pos.x--;
}

void PositionComponent::moveRight()
{
	m_pos.x++;
}

void PositionComponent::moveUp()
{
	m_pos.y--;
}

void PositionComponent::moveDown()
{
	m_pos.y++;
}