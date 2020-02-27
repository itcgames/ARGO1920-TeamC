#include "stdafx.h"
#include "TransformComponent.h"

/// <summary>
/// Constructor that takes in an x and y for position and a rotation
/// </summary>
/// <param name="t_x">x pos</param>
/// <param name="t_y">y pos</param>
/// <param name="t_rotation">rotation</param>
TransformComponent::TransformComponent(float t_x, float t_y, float t_rotation) :
	Component(ComponentType::Transform)
{
	m_pos.x = t_x;
	m_pos.y = t_y;
	m_rotation = t_rotation;
	m_leftRotation = -1;
	m_rightRotation = -1;
}

/// <summary>
/// Constructor that takes in a glm::vec2 and a rotation
/// </summary>
/// <param name="t_pos">position</param>
/// <param name="t_rotation">rotation</param>
TransformComponent::TransformComponent(glm::vec2 t_pos, float t_rotation) :
	Component(ComponentType::Transform)
{
	m_pos = t_pos;
	m_rotation = t_rotation;
	m_leftRotation = -1;
	m_rightRotation = -1;
}

/// <summary>
/// Default constructor that gives random pos and rotation of 0.0f 
/// </summary>
TransformComponent::TransformComponent(bool t_alwaysOnScreen) :
	Component(ComponentType::Transform),
	m_alwaysOnScreen(t_alwaysOnScreen)
{
	m_pos.x = glm::linearRand(50, 700);
	m_pos.y = glm::linearRand(50, 700);
	m_rotation = 0.0f;
	m_leftRotation = -1;
	m_rightRotation = -1;
}

TransformComponent::~TransformComponent()
{
}


glm::vec2 TransformComponent::getPos() const
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

void TransformComponent::setPos(glm::vec2 t_newPos)
{
	m_pos = t_newPos;
}

void TransformComponent::addPos(glm::vec2 t_displacement)
{
	m_pos += t_displacement;
}

void TransformComponent::addPos(float t_x, float t_y)
{
	m_pos.x += t_x;
	m_pos.y += t_y;
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

void TransformComponent::setLeftRotation(float t_newLeftRotation)
{
	m_leftRotation = t_newLeftRotation;
}

void TransformComponent::setRightRotation(float t_newRightRotation)
{
	m_rightRotation = t_newRightRotation;
}

float TransformComponent::getLeftRotation()
{
	return m_leftRotation;
}

float TransformComponent::getRightRotation()
{
	return m_rightRotation;
}

void TransformComponent::setAlwaysOnScreen(bool t_alwaysOnScreen)
{
	m_alwaysOnScreen = t_alwaysOnScreen;
}

bool TransformComponent::getAlwaysOnscreen()
{
	return m_alwaysOnScreen;
}


