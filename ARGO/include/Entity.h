#pragma once
#include <vector>
#include "Component.h"


class Entity
{
public:
	Entity(int t_entityIdNum);
	~Entity();
	void addComponent(Component* c);
	void removeComponent(Component* c);
	std::vector<Component*> getComponents();
	int getId() { return m_id; }
private:
	std::vector<Component*> m_components;
	int m_id;
};