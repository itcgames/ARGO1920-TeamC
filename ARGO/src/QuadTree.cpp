#include "stdafx.h"
#include "QuadTree.h"

QuadTree::QuadTree(int t_level, glm::vec2 t_position, glm::vec2 t_size) :
	m_boundsTopLeft(t_position),
	m_boundsBottomRight(t_size),
	m_level(t_level)
{
}

QuadTree::~QuadTree()
{
	clear();
}

void QuadTree::clear()
{
	m_objects.clear();
	m_nodes.clear();
}

void QuadTree::insert(Quad t_data)
{
	if (!m_nodes.empty())
	{
		int index = getIndex(t_data);
		if (index != -1)
		{
			m_nodes[index].insert(t_data);
			return;
		}
		m_objects.push_back(t_data);
		return;
	}
	m_objects.push_back(t_data);
	if (m_objects.size() > MAX_OBJECTS && m_level < MAX_LEVEL)
	{
		if (m_nodes.empty())
		{
			split();
		}

		std::vector<Quad>::iterator iter = m_objects.begin();
		while (iter != m_objects.end())
		{
			int index = getIndex(*iter);
			if (index != -1)
			{
				m_nodes[index].insert(*iter);
				iter = m_objects.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
}

void QuadTree::retrieve(std::vector<Entity*>* t_vector, Quad t_data)
{
	int index = getIndex(t_data);
	if (!m_nodes.empty())
	{
		if (index != -1)
		{
			m_nodes[index].retrieve(t_vector, t_data);
		}
		else
		{
			for (auto& node : m_nodes)
			{
				if (node.intersects(t_data))
				{
					node.retrieve(t_vector, t_data);
				}
			}
		}
	}

	for (auto& object : m_objects)
	{
		t_vector->push_back(object.data);
	}
}

int QuadTree::countAll(int count)
{
	for (auto& node : m_nodes)
	{
		count = node.countAll(count);
	}

	count += m_objects.size();
	return count;
}

int QuadTree::getIndex(Quad t_data)
{
	int index = -1;
	float verticalMidpoint = m_boundsTopLeft.x + (m_boundsBottomRight.x / 2);
	float horizontalMidpoint = m_boundsTopLeft.y + (m_boundsBottomRight.y / 2);

	// Object can completely fit within the top quadrants
	bool topQuadrant = (t_data.position.y + t_data.size.y <= horizontalMidpoint);
	// Object can completely fit within the bottom quadrants
	bool bottomQuadrant = (t_data.position.y >= horizontalMidpoint);

	// Object can completely fit within the left quadrants
	if (t_data.position.x + t_data.size.x <= verticalMidpoint)
	{
		if (topQuadrant)
		{
			index = 1;
		}
		else if (bottomQuadrant)
		{
			index = 2;
		}
	}
	// Object can completely fit within the right quadrants
	else if (t_data.position.x >= verticalMidpoint)
	{
		if (topQuadrant)
		{
			index = 0;
		}
		else if (bottomQuadrant)
		{
			index = 3;
		}
	}

	return index;
}

bool QuadTree::intersects(Quad t_data)
{
	if (m_boundsTopLeft.x > t_data.size.x + t_data.position.x || m_boundsBottomRight.x + m_boundsTopLeft.x < t_data.position.x) return false;
	if (m_boundsTopLeft.y > t_data.size.y + t_data.position.y || m_boundsBottomRight.y + m_boundsTopLeft.y < t_data.position.y) return false;
	return true;
}

void QuadTree::split()
{
	int subdividedWidth = (m_boundsBottomRight.x / 2);
	int subdividedHeight = (m_boundsBottomRight.y / 2);
	int x = m_boundsTopLeft.x;
	int y = m_boundsTopLeft.y;

	m_nodes.push_back(QuadTree(m_level + 1, glm::vec2(x + subdividedWidth, y), glm::vec2(subdividedWidth, subdividedHeight)));
	m_nodes.push_back(QuadTree(m_level + 1, glm::vec2(x, y), glm::vec2(subdividedWidth, subdividedHeight)));
	m_nodes.push_back(QuadTree(m_level + 1, glm::vec2(x, y + subdividedHeight), glm::vec2(subdividedWidth, subdividedHeight)));
	m_nodes.push_back(QuadTree(m_level + 1, glm::vec2(x + subdividedWidth, y + subdividedHeight), glm::vec2(subdividedWidth, subdividedHeight)));
}