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

enum class TileType
{
	Wall = 0,
	Floor = 1
};

class TileComponent :
	public Component
{
public:
	TileComponent(TileType t_type);
	Neighbours* getNeighbours();
	void setTileType(TileType t_type);
	TileType getTileType() const;
private:
	Neighbours m_neighbours;
	TileType m_type;
};

