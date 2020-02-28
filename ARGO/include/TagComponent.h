#pragma once
#include "Component.h"

enum class Tag
{
	Player,
	Enemy,
	PlayerBullet,
	EnemyBullet,
	Tile,
	PickUp,
	Goal,
	GlowStick,
	Grenade
};

class TagComponent : public Component
{
public:
	TagComponent(Tag t_tag);
	Tag getTag();
	void setTag(Tag t_tag);
private:
	Tag m_tag;
};

