#include "stdafx.h"
#include "TransformComponent.h"
#include <random>
#include "gtc/random.hpp"

TransformComponent::TransformComponent(float t_x, float t_y, float t_rotation) :
	Component(ComponentType::Transform)
{
	m_pos.x = t_x;
	m_pos.y = t_y;
	m_rotation = t_rotation;
}

TransformComponent::TransformComponent() :
	Component(ComponentType::Transform)
{
	m_pos.x = glm::linearRand(50, 700);
	m_pos.y = glm::linearRand(50, 700);
}

TransformComponent::~TransformComponent()
{
}

TransformComponent::Position TransformComponent::getPos() const
{
	return m_pos;
}

float TransformComponent::getRotation() const
{
	return m_rotation;
}

void TransformComponent::setPos(float t_x, float t_y)
{
	m_pos.x = t_x;
	m_pos.y = t_y;
}

void TransformComponent::setRotation(float t_newRotation)
{
	m_rotation = t_newRotation;
}

/// <summary>
/// Pass in a value to change current rotation by an amount
/// </summary>
/// <param name="t_changeRotation">change in rotation</param>
void TransformComponent::rotate(float t_changeRotation)
{
	m_rotation += t_changeRotation;
}

void TransformComponent::setX(float t_x)
{
	m_pos.x = t_x;
}

void TransformComponent::setY(float t_y)
{
	m_pos.y = t_y;
}

void TransformComponent::moveLeft()
{
	m_pos.x--;
}

void TransformComponent::moveRight()
{
	m_pos.x++;
}

void TransformComponent::moveUp()
{
	m_pos.y--;
}

void TransformComponent::moveDown()
{
	m_pos.y++;
}