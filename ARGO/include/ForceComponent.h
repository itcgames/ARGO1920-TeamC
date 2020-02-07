#pragma once
#include "Component.h"
#include <common.hpp>

class ForceComponent : public Component
{
public:
	ForceComponent(glm::vec2 t_force);
	~ForceComponent();

	glm::vec2 getForce() const;
	float getForceAngle() const;
	void setForce(float t_x, float t_y);
	void setForce(glm::vec2 t_force);
	void setForceDirection(float t_newForceDirection);
	float getMag() const;
	glm::vec2 getUnit();
	void setX(float t_x);
	void setY(float t_y);

private:
	glm::vec2 m_force;
};

