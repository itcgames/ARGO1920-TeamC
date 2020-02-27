#pragma once
#include "Component.h"

class TransformComponent : public Component
{
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
	TransformComponent(bool t_alwaysOnScreen = false);
	TransformComponent(float t_x, float t_y, float t_rotation = 0.0f);
	TransformComponent(glm::vec2 t_pos, float t_rotation = 0.0f);
	~TransformComponent();

	glm::vec2 getPos() const;
	float getRotation() const;
	void setPos(float t_x, float t_y);
	void setPos(glm::vec2 t_newPos);
	void addPos(glm::vec2 t_displacement);
	void addPos(float t_x, float t_y);
	void setRotation(float t_newRotation);
	void rotate(float t_changeRotation);
	void setX(float t_x);
	void setY(float t_y);
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void setLeftRotation(float t_newLeftRotation);
	void setRightRotation(float t_newRightRotation);
	float getLeftRotation();
	float getRightRotation();
	void setAlwaysOnScreen(bool t_alwaysOnScreen);
	bool getAlwaysOnscreen();

private:
	glm::vec2 m_pos;
	float m_rotation;
	// rotation for the left thumb stick
	float m_leftRotation;
	// rotation for the right thumb stick
	float m_rightRotation;
	bool m_alwaysOnScreen = false;
};

