#pragma once
#include "Entity.h"
#include "BaseSystem.h"
#include "RenderSystem.h"
#include "SDL_image.h"
#include "ParticleEmitterComponent.h"
#include "PrimitiveComponent.h"
#include "ColourComponent.h"
#include "EventManager.h"
#include "TransformComponent.h"
#include "ParticleSystem.h"


class ParticleManager
{
public:
	ParticleManager(EventManager& t_eventManager, ParticleSystem& t_particleSystem);
	void init(SDL_Renderer* t_renderer);
	void createParticleEmitter(Entity& t_entity);
	void placeEmitter(glm::vec2 t_pos, int t_pickUpType);
	void update(float t_dt);
	void nextAvailableEmitter();
	void render(SDL_Renderer* t_renderer, RenderSystem* t_system);
private:
	static const int EMITTER_POOL_SIZE = 1;
	Entity m_particleEmitter[EMITTER_POOL_SIZE];
	SDL_Renderer* m_renderer;
	ParticleSystem& m_particleSystem;
	int m_currentEmitter{ 0 };
};

