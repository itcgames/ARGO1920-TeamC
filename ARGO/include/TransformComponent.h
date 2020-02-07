#pragma once
#include "Component.h"

class TransformComponent : public Component
{
	struct Position
	{
		float x = 0.0f;
		float y = 0.0f;
	};

	////might be good to add this to get rotation x&y
	//struct Orientation
	//{
	//	float x;
	//	float y;
	//};

	//Uncomment if needed
	//struct Scale
	//{
	//	float x;
	//	float y;
	//};

public:
	TransformComponent(float t_x, float t_y, float t_rotation);
	TransformComponent();
	~TransformComponent();

	Position getPos() const;
	float getRotation() const;
	void setPos(float t_x, float t_y);
	void setRotation(float t_newRotation);
	void rotate(float t_changeRotation);
	void setX(float t_x);
	void setY(float t_y);
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	bool m_movingRight = true;
	bool m_movingUp = false;

private:
	Position m_pos;
	float m_rotation = 0.0f;
};

