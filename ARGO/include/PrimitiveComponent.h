#pragma once
#include "Component.h"
class PrimitiveComponent :
	public Component
{

public:
	PrimitiveComponent(glm::vec2 t_size, glm::vec2 t_pos);
	PrimitiveComponent();
	~PrimitiveComponent();

	void setSize(glm::vec2 t_size);
	glm::vec2 getSize();
	void setPosition(glm::vec2 t_pos);
	glm::vec2 getPosition();

private:
	glm::vec2 m_size{ 4,4 };
	glm::vec2 m_position{ 0,0 };
};

