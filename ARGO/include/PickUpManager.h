#pragma once
#include "Entity.h"
#include "BaseSystem.h"
#include "RenderSystem.h"
#include "SDL_image.h"
#include "PickUpFactory.h"
#include "EventManager.h"
#include "CollisionSystem.h"


class PickUpManager
{
public:
	PickUpManager(EventManager& t_eventManager, CollisionSystem& t_collisionSystem);
	void init(SDL_Renderer* t_renderer);
	void randomPickupSpawn(const EnemyKilled& t_event);
	void removePickup(const PickupGrabbed& t_event);
	void placePickup(glm::vec2 t_pos, int t_pickUpType);
	void update(float t_dt);
	void nextAvailablePickup();
	void reset();
	void render(SDL_Renderer* t_renderer, RenderSystem* t_system);
	Entity(&getPickups())[Utilities::PICKUP_POOL_SIZE];
private:
	Entity m_pickUps[Utilities::PICKUP_POOL_SIZE];
	SDL_Renderer* m_renderer;
	int m_currentPickup{ 0 };
	CollisionSystem& m_collisionSystem;

	const int PICKUP_SPAWN_CHANCE = 5;
};

