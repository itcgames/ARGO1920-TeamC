#include "stdafx.h"
#include "PickUpManager.h"
#include "AssetManager.h"


PickUpManager::PickUpManager(EventManager& t_eventManager, CollisionSystem& t_collisionSystem) :
	m_collisionSystem(t_collisionSystem)
{
	t_eventManager.subscribeToEvent<PickupGrabbed>(std::bind(&PickUpManager::removePickup, this, std::placeholders::_1));
}
///<summary>
/// Initialise the Manager. Sets the renderer.
///</summary>
void PickUpManager::init(SDL_Renderer* t_renderer)
{
	m_renderer = t_renderer;
	PickUpFactory* factory = new PickUpFactory(m_renderer);
	for (int i = 0; i < PICKUP_POOL_SIZE; i++)
	{
		factory->createDrop(2, m_pickUps[i]);
		TransformComponent* transformComp = static_cast<TransformComponent*>(m_pickUps[i].getComponent(ComponentType::Transform));
		transformComp->setPos(glm::vec2(100 + (90 * i), 100));
		HealthComponent* healthComp = static_cast<HealthComponent*>(m_pickUps[i].getComponent(ComponentType::Health));
		healthComp->addHealth(0);
	}
	delete factory;
}

///<summary>
/// Removes and kills a component if it collides with a player.
///</summary>
void PickUpManager::removePickup(const PickupGrabbed& t_event)
{
	HealthComponent* healthComp = static_cast<HealthComponent*>(t_event.pickup->getComponent(ComponentType::Health));
	if (healthComp->isAlive())
	{
		TransformComponent* transformComp = static_cast<TransformComponent*>(t_event.pickup->getComponent(ComponentType::Transform));
		transformComp->setPos(-1000, -1000);

		healthComp->setHealth(0);
		nextAvailablePickup();
	}

}

///<summary>
/// If there is a pickup available then it places it at the desired position of the desired type.Need to add component removal first.
///</summary>
void PickUpManager::placePickup(glm::vec2 t_pos, int t_pickUpType)
{
	if (m_currentPickup != -1)
	{
		PickUpFactory* factory = new PickUpFactory(m_renderer);
		m_pickUps[m_currentPickup].removeAllComponents();
		factory->createDrop(t_pickUpType, m_pickUps[m_currentPickup]);
		TransformComponent* transformComp = static_cast<TransformComponent*>(m_pickUps[m_currentPickup].getComponent(ComponentType::Transform));
		transformComp->setPos(t_pos);
		HealthComponent* healthComp = static_cast<HealthComponent*>(m_pickUps[m_currentPickup].getComponent(ComponentType::Health));
		healthComp->addHealth(1);
		delete factory;
	}
}
///<summary>
/// If a Pickup is alive then it rotates it.
///</summary>
void PickUpManager::update(float t_dt)
{
	for (auto& pickUp : m_pickUps)
	{
		HealthComponent* healthComp = static_cast<HealthComponent*>(pickUp.getComponent(ComponentType::Health));
		if (healthComp->isAlive())
		{
			m_collisionSystem.update(pickUp);
			TransformComponent* transformComp = static_cast<TransformComponent*>(pickUp.getComponent(ComponentType::Transform));
			PickUpComponent* pickUpComp = static_cast<PickUpComponent*>(pickUp.getComponent(ComponentType::PickUp));
			transformComp->setRotation(transformComp->getRotation() + pickUpComp->getRotationSpeed() * t_dt);
		}
	}
}


///<summary>
///Function that finds the next available pickup in the pool to spawn. If one can't be found then the m_currentPickup is set to -1 until one is removed and set to dead.
///</summary>
void PickUpManager::nextAvailablePickup()
{

	if (m_currentPickup < PICKUP_POOL_SIZE - 1 && m_currentPickup != -1)
	{
		HealthComponent* healthComp = static_cast<HealthComponent*>(m_pickUps[m_currentPickup + 1].getComponent(ComponentType::Health));
		if (!healthComp->isAlive())
		{
			m_currentPickup++;
		}
		else {
			bool availableEntity = false;
			for (int i = 0; i < PICKUP_POOL_SIZE; i++)
			{
				healthComp = static_cast<HealthComponent*>(m_pickUps[i].getComponent(ComponentType::Health));
				if (!healthComp->isAlive())
				{
					m_currentPickup = i;
					availableEntity = true;
				}
			}
			if (!availableEntity)
			{
				m_currentPickup = -1;
			}
		}
	}

	else {
		for (int i = 0; i < PICKUP_POOL_SIZE; i++)
		{
			HealthComponent* healthComp = static_cast<HealthComponent*>(m_pickUps[i].getComponent(ComponentType::Health));
			if (!healthComp->isAlive())
			{
				m_currentPickup = i;
			}
		}
	}

}
///<summary>
/// If a Pickup is alive then it renders it.
///</summary>
void PickUpManager::render(SDL_Renderer* t_renderer, RenderSystem* t_system)
{
	for (auto& pickUp : m_pickUps)
	{
		HealthComponent* healthComp = static_cast<HealthComponent*>(pickUp.getComponent(ComponentType::Health));
		if (healthComp->isAlive())
		{
			t_system->render(t_renderer, pickUp);
		}
	}
}
