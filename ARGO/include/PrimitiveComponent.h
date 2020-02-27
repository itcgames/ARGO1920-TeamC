#pragma once
#include "Component.h"
class PrimitiveComponent :
	public Component
{

public:
	PrimitiveComponent(glm::vec2 t_size, glm::vec2 t_pos, bool t_staticPosition = false, float t_minimumSize = 4, float t_maximumSize = 40);
	PrimitiveComponent();
	~PrimitiveComponent();

	void setSize(glm::vec2 t_size);
	glm::vec2 getSize();
	void setPosition(glm::vec2 t_pos);
	glm::vec2 getPosition();
	void setStaticPosition(bool t_staticPosition);
	bool getStaticPosition();
	float getMinimumSize();
	void setMinimumSize(float t_minimum);
	float getMaximumSize();
	void setMaximumSize(float t_maximum);

private:
	glm::vec2 m_size{ 4,4 };
	glm::vec2 m_position{ 0,0 };
	float m_minimumSize = 4;
	float m_maximumSize = 4;
	bool m_staticPosition = false;
};

