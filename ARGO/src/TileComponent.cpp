#include "stdafx.h"
#include "TileComponent.h"

TileComponent::TileComponent() :
	Component(ComponentType::Tile),
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
