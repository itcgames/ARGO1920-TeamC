#pragma once
#include "Component.h"
enum class AvatarState
{
	Player,
	Skull
};

class HUDComponent :
	public Component
{
public:
	HUDComponent(float t_index);
	glm::vec2 getHUDPosition();
	void setHUDPosition(glm::vec2 t_position);

	int getIndex();

	glm::vec2 getHealthOffset();
	glm::vec2 getMaxHealthSize();
	glm::vec2 getCurrentHealthSize();
	void setCurrentHealthSize(float t_sizeOfX);

	glm::vec2 getAmmoOffset();
	glm::vec2 getMaxAmmoSize();
	glm::vec2 getCurrentAmmoSize();
	void setCurrentAmmoSize(float t_sizeOfX);

	glm::vec2 getHealthTextOffset();
	float getHealthTextSize();

	glm::vec2 getAmmoTextOffset();
	float getAmmoTextSize();

	glm::vec2 getAvatarOffset();
	AvatarState getAvatarState();
	void setAvatarState(AvatarState t_state);


private:
	int m_playerIndex{ 0 };
	glm::vec2 m_hudPosition{ 100,10 };

	glm::vec2 m_healthBarOffset{ 95,0 };
	glm::vec2 m_healthBarSizeMax{ 200,37 };
	glm::vec2 m_healthBarSizeCurrent{ 200,37 };

	glm::vec2 m_ammoBarOffset{ 95,42 };
	glm::vec2 m_ammoBarSizeMax{ 200,32 };
	glm::vec2 m_ammoBarSizeCurrent{ 200,32 };

	glm::vec2 m_healthTextOffset{ 150,0 };
	float m_healthTextSize{ 25 };

	glm::vec2 m_ammoTextOffset{ 150,42 };
	float m_ammoTextSize{ 20 };

	glm::vec2 m_avatarOffset{ 2,5 };
	AvatarState m_avatarIconState = AvatarState::Player;
	
};

