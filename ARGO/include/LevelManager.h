#pragma once
#include "Entity.h"
#include "Utilities.h"
#include "TagComponent.h"
#include "ColliderAABBComponent.h"
#include "TileComponent.h"
#include "HealthComponent.h"
#include "FlowFieldComponent.h"
#include "lightFieldComponent.h"
#include "PathingComponent.h"
#include "BaseSystem.h"
#include "RenderSystem.h"
#include "ProjectileManager.h"

class LevelManager
{
public:
	LevelManager(SDL_Renderer* t_renderer, Entity(&t_players)[Utilities::S_MAX_PLAYERS], RenderSystem& t_renderSystem, ProjectileManager& t_projectileManager);
	void setupLevel();
	void update(BaseSystem* t_system);
	void checkWallDamage();
	void render(SDL_Renderer* t_renderer, RenderSystem* t_system);
	void renderLight(SDL_Renderer* t_renderer, RenderSystem* t_system);
	void setToWall(Entity& t_entity);
	void setToFloor(Entity& t_entity);
	void createRoom(glm::vec2 t_startPosition, int t_width, int t_height);
	Entity* findAtPosition(glm::vec2 t_position);
	void resetPathing();

private:
	void setTileNeighbours();
	void resetFields();
	void generateFlowField();
	void setNeighbourWeights(Entity* t_entity, std::vector<Entity*>& t_queue);
	void setTileWeight(Entity* t_entity, Entity* t_closestNeighbour, std::vector<Entity*>& t_queue, int t_newWeight);
	void generateLightField(); 
	void setTileLight(Entity* t_entity, std::vector<Entity*>& t_queue, int t_newWeight);
	void setNeighbourLights(Entity* t_entity, std::vector<Entity*>& t_queue);

	std::vector<Entity> m_levelTiles;
	Entity(&m_players)[Utilities::S_MAX_PLAYERS];
	SDL_Renderer* m_renderer;
	RenderSystem& m_renderSystem;
	ProjectileManager& m_projectilemanager;

	const int LIGHT_LOST_PER_TILE = 32;
	const int LIGHT_LOST_PER_CORNER = 45; //LIGHT_LOST_PER_TILE(32) / tile size(64) * distance to corner tiles (90.5f)
	const int MAX_LIGHT_WEIGHT = 256;
};