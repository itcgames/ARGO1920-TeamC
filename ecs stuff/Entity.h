#pragma once
#include <vector>
#include <memory>
#include "Component.h"


class Entity
{
public:
	Entity(int t_entityIdNum);
	~Entity();
	//void addComponent(Component* c);
	void addSmartComponent(std::unique_ptr<Component> t_c);
	void removeComponent(Component* c);
	//std::vector<Component*> getComponents();
	Component* getComponent(Component::ComponentType t_type);
	int getId() { return m_id; }
private:
	//std::vector<Component*> m_components;

	std::vector<std::unique_ptr<Component>> m_smartComps;

	int m_id;
};