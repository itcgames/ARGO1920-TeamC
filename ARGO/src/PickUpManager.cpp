#include "stdafx.h"
#include "PickUpManager.h"
#include "AssetManager.h"


PickUpManager::PickUpManager(EventManager& t_eventManager, CollisionSystem& t_collisionSystem):
	m_collisionSystem(t_collisionSystem)
{
	t_eventManager.subscribeToEvent<PickupGrabbed>(std::bind(&PickUpManager::removePickup, this, std::placeholders::_1));
}

void PickUpManager::init(SDL_Renderer* t_renderer)
{
	m_renderer = t_renderer;
	PickUpFactory* factory = new PickUpFactory(t_renderer);
	for (int i = 0; i < PICKUP_POOL_SIZE; i++)
	{
		factory->createDrop(1,m_pickUps[i]);
	}
	delete factory;
}

void PickUpManager::removePickup(const PickupGrabbed& t_event)
{

}

void PickUpManager::update(float t_dt)
{
	for (auto& pickUp : m_pickUps)
	{
		m_collisionSystem.update(pickUp);
		TransformComponent* inputComp = static_cast<TransformComponent*>(pickUp.getComponent(ComponentType::Transform));
		PickUpComponent* pickUpComp = static_cast<PickUpComponent*>(pickUp.getComponent(ComponentType::PickUp));
		inputComp->setRotation(inputComp->getRotation() + pickUpComp->getRotationSpeed());
	}
}

void PickUpManager::render(SDL_Renderer* t_renderer, RenderSystem* t_system)
{
	for (auto& pickUp : m_pickUps)
	{
		t_system->render(t_renderer, pickUp);
	}
}
