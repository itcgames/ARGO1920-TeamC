#include "stdafx.h"
#include "HUDComponent.h"

HUDComponent::HUDComponent(float t_index):
	Component(ComponentType::HUD),
	m_playerIndex(t_index)
{

}

glm::vec2 HUDComponent::getHUDPosition()
{
	return m_hudPosition;
}

void HUDComponent::setHUDPosition(glm::vec2 t_position)
{
	m_hudPosition = t_position;
}

int HUDComponent::getIndex()
{
	return m_playerIndex;
}

glm::vec2 HUDComponent::getHealthOffset()
{
	return m_healthBarOffset;
}

glm::vec2 HUDComponent::getMaxHealthSize()
{
	return m_healthBarSizeMax;
}

glm::vec2 HUDComponent::getCurrentHealthSize()
{
	return m_healthBarSizeCurrent;
}

void HUDComponent::setCurrentHealthSize(float t_sizeOfX)
{
	m_healthBarSizeCurrent.x = t_sizeOfX;
}

glm::vec2 HUDComponent::getAmmoOffset()
{
	return m_ammoBarOffset;
}

glm::vec2 HUDComponent::getMaxAmmoSize()
{
	return m_ammoBarSizeMax;
}

glm::vec2 HUDComponent::getCurrentAmmoSize()
{
	return m_ammoBarSizeCurrent;
}

void HUDComponent::setCurrentAmmoSize(float t_sizeOfX)
{
	m_ammoBarSizeCurrent.x = t_sizeOfX;
}

glm::vec2 HUDComponent::getHealthTextOffset()
{
	return m_healthTextOffset;
}

float HUDComponent::getHealthTextSize()
{
	return m_healthTextSize;
}

glm::vec2 HUDComponent::getAmmoTextOffset()
{
	return m_ammoTextOffset;
}

float HUDComponent::getAmmoTextSize()
{
	return m_ammoTextSize;
}

glm::vec2 HUDComponent::getAvatarOffset()
{
	return m_avatarOffset;
}

AvatarState HUDComponent::getAvatarState()
{
	return m_avatarIconState;
}

void HUDComponent::setAvatarState(AvatarState t_state)
{
	m_avatarIconState = t_state;
}
