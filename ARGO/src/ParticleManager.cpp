#include "stdafx.h"
#include "ParticleManager.h"

ParticleManager::ParticleManager(EventManager& t_eventManager, ParticleSystem& t_particleSystem) :
	m_particleSystem(t_particleSystem)
{
	t_eventManager.subscribeToEvent<EnemyKilled>(std::bind(&ParticleManager::placeEmitterEnemyDeath, this, std::placeholders::_1));
	t_eventManager.subscribeToEvent<PlayerKilled>(std::bind(&ParticleManager::placeEmitterPlayerDeath, this, std::placeholders::_1));
	for (auto& emitter : m_particleEmitter)
	{
		createParticleEmitter(emitter);
	}
}
/// <summary>
/// Function Called to Set up Particle Emitters.
/// </summary>
/// <param name="t_entity"></param>
void ParticleManager::createParticleEmitter(Entity& t_entity)
{
	t_entity.addComponent(new TransformComponent(true));
	TransformComponent* transComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
	t_entity.addComponent(new ColourComponent());
	t_entity.addComponent(new PrimitiveComponent());
	PrimitiveComponent* primComp = static_cast<PrimitiveComponent*>(t_entity.getComponent(ComponentType::Primitive));
	t_entity.addComponent(new ParticleEmitterComponent(false, Utilities::PARTICLE_PERCENTAGE_SPEED_OFFSET, Utilities::PARTICLE_BASE_SPEED,
		Utilities::PARTICLE_MAX_PARTICLES, Utilities::PARTICLE_TIME_TO_KILL_PARTICLE,
		Utilities::PARTICLES_PER_SECOND));
}

/// <summary>
/// Function called On Enemy Death
/// </summary>
/// <param name="t_event"></param>
void ParticleManager::placeEmitterEnemyDeath(const EnemyKilled& t_event)
{
	TransformComponent* emitterTransComp = static_cast<TransformComponent*>(m_particleEmitter[m_currentEmitter].getComponent(ComponentType::Transform));
	TransformComponent* enemyTransComp = static_cast<TransformComponent*>(t_event.enemy->getComponent(ComponentType::Transform));
	emitterTransComp->setPos(enemyTransComp->getPos());
	ParticleEmitterComponent* emitterComp = static_cast<ParticleEmitterComponent*>(m_particleEmitter[m_currentEmitter].getComponent(ComponentType::ParticleEmitter));
	emitterComp->setEmitting(true);
	ColourComponent* colourComp = static_cast<ColourComponent*>(m_particleEmitter[m_currentEmitter].getComponent(ComponentType::Colour));
	//Converting uVec4 into Colour structs
	Colour primaryColour{ Utilities::PRIMARY_ENEMY_DEATH_COLOUR.x, Utilities::PRIMARY_ENEMY_DEATH_COLOUR.y, Utilities::PRIMARY_ENEMY_DEATH_COLOUR.z, Utilities::PRIMARY_ENEMY_DEATH_COLOUR.a };
	Colour secondaryColour{ Utilities::SECONDARY_ENEMY_DEATH_COLOUR.x, Utilities::SECONDARY_ENEMY_DEATH_COLOUR.y, Utilities::SECONDARY_ENEMY_DEATH_COLOUR.z, Utilities::SECONDARY_ENEMY_DEATH_COLOUR.a };
	colourComp->setColor(primaryColour);
	colourComp->setSecondaryColour(secondaryColour);
	PrimitiveComponent* primComp = static_cast<PrimitiveComponent*>(m_particleEmitter[m_currentEmitter].getComponent(ComponentType::Primitive));
	primComp->setMaximumSize(Utilities::ENEMY_DEATH_MAX_PARTICLE_SIZE);
	primComp->setMinimumSize(Utilities::ENEMY_DEATH_MIN_PARTICLE_SIZE);
	nextAvailableEmitter();
}
/// <summary>
/// Function called on player Death. 
/// </summary>
/// <param name="t_event"></param>
void ParticleManager::placeEmitterPlayerDeath(const PlayerKilled& t_event)
{
	TransformComponent* emitterTransComp = static_cast<TransformComponent*>(m_particleEmitter[m_currentEmitter].getComponent(ComponentType::Transform));
	TransformComponent* playerTransComp = static_cast<TransformComponent*>(t_event.player->getComponent(ComponentType::Transform));
	emitterTransComp->setPos(playerTransComp->getPos());
	ParticleEmitterComponent* emitterComp = static_cast<ParticleEmitterComponent*>(m_particleEmitter[m_currentEmitter].getComponent(ComponentType::ParticleEmitter));
	emitterComp->setEmitting(true);
	ColourComponent* colourComp = static_cast<ColourComponent*>(m_particleEmitter[m_currentEmitter].getComponent(ComponentType::Colour));
	//Converting uVec4 into Colour structs
	Colour primaryColour{ Utilities::PRIMARY_PLAYER_DEATH_COLOUR.x, Utilities::PRIMARY_PLAYER_DEATH_COLOUR.y, Utilities::PRIMARY_PLAYER_DEATH_COLOUR.z, Utilities::PRIMARY_PLAYER_DEATH_COLOUR.a };
	Colour secondaryColour{ Utilities::SECONDARY_PLAYER_DEATH_COLOUR.x, Utilities::SECONDARY_PLAYER_DEATH_COLOUR.y, Utilities::SECONDARY_PLAYER_DEATH_COLOUR.z, Utilities::SECONDARY_PLAYER_DEATH_COLOUR.a };
	colourComp->setColor(primaryColour);
	colourComp->setSecondaryColour(secondaryColour);
	PrimitiveComponent* primComp = static_cast<PrimitiveComponent*>(m_particleEmitter[m_currentEmitter].getComponent(ComponentType::Primitive));
	primComp->setMaximumSize(Utilities::PLAYER_DEATH_MAX_PARTICLE_SIZE);
	primComp->setMinimumSize(Utilities::PLAYER_DEATH_MIN_PARTICLE_SIZE);
	nextAvailableEmitter();
}

/// <summary>
/// Updates all Particle Emitters
/// </summary>
/// <param name="t_dt"></param>
void ParticleManager::update(float t_dt)
{
	for (auto& emitter : m_particleEmitter)
	{
		m_particleSystem.update(emitter, t_dt);
	}
}
/// <summary>
/// Gets the Next Avialable Emitter> Loops back  when it reaches the end.
/// </summary>
void ParticleManager::nextAvailableEmitter()
{
	m_currentEmitter++;
	if (m_currentEmitter == Utilities::PARTICLE_MAX_EMITTER_POOL)
	{
		m_currentEmitter = 0;
	}
}

/// <summary>
/// Renders all particles.
/// </summary>
/// <param name="t_renderer"></param>
/// <param name="t_system"></param>
void ParticleManager::render(SDL_Renderer* t_renderer, RenderSystem* t_system)
{
	for (auto& emitter : m_particleEmitter)
	{
		t_system->render(t_renderer, emitter);
	}
}
