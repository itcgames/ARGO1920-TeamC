#pragma once
#include <algorithm>
#include <vector>
#include <memory>
#include "Component.h"
#include "CompEnums.h"


class Entity
{
public:
	Entity();
	//Entity(int t_entityIdNum);
	~Entity();

	void addComponent(Component* t_c);
	void removeCompType(ComponentType t_type);

	Component* getComponent(ComponentType t_type);
	bool hasComponentType(ComponentType t_type) const;
	//int getId() { return m_id; }
private:
	std::vector<Component*> m_components;

	//might want to have an enum for entity types
	//int m_id;
};