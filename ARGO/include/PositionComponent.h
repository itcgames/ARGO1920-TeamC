#pragma once
#include "Component.h"
class PositionComponent : public Component
{
	struct Position
	{
		float x;
		float y;
	};

public:
	PositionComponent(float t_x, float t_y);
	PositionComponent();
	~PositionComponent();

	Position getPos();
	void setPos(float t_x, float t_y);
	void setX(float t_x);
	void setY(float t_y);
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	bool m_right = true;
	bool m_up = false;
private:
	Position m_pos;
};

