#pragma once
#include "CompEnums.h"

class Component
{
public:
	Component(ComponentType t_type);
	virtual ~Component() = 0;
	ComponentType getType() const { return m_type; }

private:
	ComponentType m_type;
};

