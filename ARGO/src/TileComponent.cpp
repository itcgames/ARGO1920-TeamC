#include "stdafx.h"
#include "TileComponent.h"

TileComponent::TileComponent(TileType t_type) :
	Component(ComponentType::Tile),
	m_type(t_type),
	m_neighbours{nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr}
{
}

Neighbours* TileComponent::getNeighbours()
{
	return &m_neighbours;
}

void TileComponent::setTileType(TileType t_type)
{
	m_type = t_type;
}

TileType TileComponent::getTileType() const
{
	return m_type;
}
