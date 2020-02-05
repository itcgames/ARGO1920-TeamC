#pragma once

enum class TileType
{
	Ground,
	Wall
};

class Tile // Extend Entity
{
public:
	Tile(glm::vec2 t_position, float t_size, TileType t_type);



//accessors
	//Position - Chenge to position component
	glm::vec2 getPosition() { return m_position; };
	void setPosition(glm::vec2 t_position) { m_position = t_position; };
	//Type
	TileType getType() { return m_type; };
	void setType(TileType t_type) { m_type = t_type; };
	//Size - change to render pomponent
	int getSize() { return m_tileSize; };
	//Neighbours
	std::vector<Tile*>* getNeightbours() { return &m_neighbours; };
private:
	glm::vec2 m_position;
	TileType m_type;

	float m_tileSize;
	std::vector<Tile*> m_neighbours;
};

