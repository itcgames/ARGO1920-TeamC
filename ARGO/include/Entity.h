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

	Component* getComponent(ComponentType t_type);
	bool hasComponentType(ComponentType t_type) const;
	std::vector<Component*>& getAllComps();
private:
	//used to reserve space in the components vector 
	//10 might be too much perhaps
	const static int S_MAX_COMPS = 10;
	std::vector<Component*> m_components;
};