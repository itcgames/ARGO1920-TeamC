#pragma once
#include "Component.h"
class ColliderAABB : public Component
{
public:
	ColliderAABB(glm::vec2 t_bounds);
	glm::vec2 getBounds();
	void setBounds(glm::vec2 t_bounds);

private:
	glm::vec2 m_bounds;
};

