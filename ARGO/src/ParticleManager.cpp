#include "stdafx.h"
#include "ParticleManager.h"

ParticleManager::ParticleManager(EventManager& t_eventManager, ParticleSystem& t_particleSystem):
	m_particleSystem(t_particleSystem)
{
	//t_eventManager.subscribeToEvent<Ene>(std::bind(&PickUpManager::removePickup, this, std::placeholders::_1));
	for (auto& emitter : m_particleEmitter)
	{
		createParticleEmitter(emitter);
	}
}

void ParticleManager::createParticleEmitter(Entity& t_entity)
{
	t_entity.addComponent(new TransformComponent(true));
	TransformComponent* transComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
	transComp->setPos(glm::vec2(100, 100));
	t_entity.addComponent(new ColourComponent(static_cast<Uint8>(0), static_cast<Uint8>(125), static_cast<Uint8>(50), 255));
	t_entity.addComponent(new PrimitiveComponent());
	PrimitiveComponent* primComp = static_cast<PrimitiveComponent*>(t_entity.getComponent(ComponentType::Primitive));
	primComp->setSize(glm::vec2(5,5));
	t_entity.addComponent(new ParticleEmitterComponent(glm::vec2(100, 100),true,90,180.0f,16.0f,400,4000,10,false,0.0f,EmitterType::Burst));
	ParticleEmitterComponent* emitterComp = static_cast<ParticleEmitterComponent*>(t_entity.getComponent(ComponentType::ParticleEmitter));
}

void ParticleManager::update(float t_dt)
{
	for (auto& emitter : m_particleEmitter)
	{
		m_particleSystem.update(emitter, t_dt);
	}
}

void ParticleManager::render(SDL_Renderer* t_renderer, RenderSystem* t_system)
{
	for (auto& emitter : m_particleEmitter)
	{
		t_system->render(t_renderer,emitter);
	}
}
