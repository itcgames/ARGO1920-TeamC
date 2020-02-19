#pragma once
#include <Entity.h>

struct Quad
{
	Entity* data;
	glm::vec2 position;
	glm::vec2 size;
};

class QuadTree
{
public:
	QuadTree(int t_level, glm::vec2 t_position, glm::vec2 t_size);
	~QuadTree();
	void clear();
	void insert(Quad t_data);
	void retrieve(std::vector<Entity*>* t_vector, Quad t_data);
	int countAll(int count);
private:
	int getIndex(Quad t_data);
	bool intersects(Quad t_data);
	void split();

	glm::vec2 m_boundsTopLeft;
	glm::vec2 m_boundsBottomRight;
	int m_level;

	std::vector<Quad> m_objects;
	std::vector<QuadTree> m_nodes;

	static const int MAX_OBJECTS = 4;
	static const int MAX_LEVEL = 10;
};

