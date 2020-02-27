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
	void createParticleEmitter(Entity& t_entity);
	void placeEmitterEnemyDeath(const EnemyKilled& t_event);
	void placeEmitterPlayerDeath(const PlayerKilled& t_event);
	void placeEmitterExplosion(const Explosion& t_event);
	void update(float t_dt);
	void nextAvailableEmitter();
	void render(SDL_Renderer* t_renderer, RenderSystem* t_system);
private:
	Entity m_particleEmitter[Utilities::PARTICLE_MAX_EMITTER_POOL];
	SDL_Renderer* m_renderer;
	ParticleSystem& m_particleSystem;
	int m_currentEmitter{ 0 };
};
