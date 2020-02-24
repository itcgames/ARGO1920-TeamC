#pragma once
#include "Component.h"

enum class Tag
{
	Player,
	Enemy,
	PlayerBullet,
	EnemyBullet,
	Tile,
	PickUp
};

class TagComponent : public Component
{
public:
	TagComponent(Tag t_tag);
	Tag getTag();
private:
	Tag m_tag;
};

