#pragma once
#include "Component.h"
class ColliderAABBComponent : public Component
{
public:
	ColliderAABBComponent(glm::vec2 t_bounds);
	glm::vec2 getBounds();
	void setBounds(glm::vec2 t_bounds);

private:
	glm::vec2 m_bounds;
};

