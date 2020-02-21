#pragma once
#include "Entity.h"
#include "Utilities.h"
#include "TagComponent.h"
#include "ColliderAABBComponent.h"
#include "TileComponent.h"
#include "HealthComponent.h"
#include "BaseSystem.h"
#include "RenderSystem.h"

class LevelManager
{
public:
	LevelManager(SDL_Renderer* t_renderer);
	void setupLevel();
	void resetLevel();
	void update(BaseSystem* t_system);
	void checkWallDamage();
	void render(SDL_Renderer* t_renderer, RenderSystem* t_system);
	void setToWall(Entity& t_entity);
	void setToFloor(Entity& t_entity);
	void createRoom(glm::vec2 t_startPosition, int t_width, int t_height);
private:
	void setTileNeighbours();
	Entity* findAtPosition(glm::vec2 t_position);

	std::vector<Entity> m_levelTiles;
	SDL_Renderer* m_renderer;
};

