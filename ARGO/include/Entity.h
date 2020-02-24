#pragma once
#include <vector>
#include <memory>
#include "Component.h"
#include "CompEnums.h"
#include <stdexcept>

class Entity
{
public:
	Entity();
	~Entity();

	void addComponent(Component* t_component);
	void removeCompType(ComponentType t_type);
	void removeAllComponents();
	void nullAllComponents();

	Component* getComponent(ComponentType t_type) const;

private:
	std::vector<Component*> m_components;
};