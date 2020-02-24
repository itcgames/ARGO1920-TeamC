#include "stdafx.h"
#include "PickUpFactory.h"


PickUpFactory::PickUpFactory(SDL_Renderer* t_renderer):
	m_renderer(t_renderer)
{
}

void PickUpFactory::createPlayer(Entity& t_entity)
{
}

void PickUpFactory::createEnemy(int choice, Entity& t_entity)
{
}

///<summary>
///Creating a Drop, pass in what enemy is desired. 1 - Ammo, 2 - Health and a reference to an entity. The required components and values will be added. 
///All values are set in the FactoryStatSheet.
///</summary>
void PickUpFactory::createDrop(int choice, Entity& t_entity)
{
	switch (choice)
	{
	case 1:
		t_entity.addComponent(new TransformComponent(FactoryStatSheet::AMMO_POSITION));
		t_entity.addComponent(new TagComponent(Tag::PickUp));
		t_entity.addComponent(new VisualComponent("AmmoBox.png", m_renderer));
		t_entity.addComponent(new ColliderCircleComponent(FactoryStatSheet::AMMO_RADIUS));
		t_entity.addComponent(new HealthComponent(FactoryStatSheet::AMMO_HP, FactoryStatSheet::AMMO_HP));
		t_entity.addComponent(new PickUpComponent(FactoryStatSheet::AMMO_TYPE_ID,FactoryStatSheet::AMMO_ROTATING,FactoryStatSheet::AMMO_ROTATION_SPEED,
			FactoryStatSheet::HEALTH_AMOUNT_CHANGE,FactoryStatSheet::AMMO_AMOUNT_CHANGE));
		break;
	case 2:
		t_entity.addComponent(new TransformComponent(FactoryStatSheet::HEALTH_POSITION));
		t_entity.addComponent(new TagComponent(Tag::PickUp));
		t_entity.addComponent(new VisualComponent("HealthBox.png", m_renderer));
		t_entity.addComponent(new ColliderCircleComponent(FactoryStatSheet::HEALTH_RADIUS));
		t_entity.addComponent(new HealthComponent(FactoryStatSheet::HEALTH_HP, FactoryStatSheet::HEALTH_HP));
		t_entity.addComponent(new PickUpComponent(FactoryStatSheet::HEALTH_TYPE_ID, FactoryStatSheet::HEALTH_ROTATING, FactoryStatSheet::HEALTH_ROTATION_SPEED,
			FactoryStatSheet::HEALTH_AMOUNT_CHANGE, FactoryStatSheet::AMMO_AMOUNT_CHANGE));
		break;
	case 3:
		break;
	}
}
