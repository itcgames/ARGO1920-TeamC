#pragma once
#include "Component.h"
#include "Entity.h"
struct Neighbours
{
	Entity* top;
	Entity* bottom;
	Entity* left;
	Entity* right;
	Entity* topLeft;
	Entity* topRight;
	Entity* bottomLeft;
	Entity* bottomRight;
};

class TileComponent :
	public Component
{
public:
	TileComponent();
	Neighbours* getNeighbours();
private:
	Neighbours m_neighbours;
};

