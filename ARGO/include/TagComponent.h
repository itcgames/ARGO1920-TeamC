#pragma once
#include "Component.h"

enum class Tag
{
	Player,
	Enemy,
	PlayerBullet,
	EnemyBullet,
	Tile,
	Goal
};

class TagComponent : public Component
{
public:
	TagComponent(Tag t_tag);
	Tag getTag();
	void setTag(int t_tag);
private:
	Tag m_tag;
};

