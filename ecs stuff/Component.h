#pragma once
class Component
{
public:
	enum class ComponentType
	{
		Health,
		Position,
		Input
	};

	Component(ComponentType t_type);
	virtual ~Component() = 0;
	ComponentType getType() { return m_type; }

private:

	ComponentType m_type;
};

