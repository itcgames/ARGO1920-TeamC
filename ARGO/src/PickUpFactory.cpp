#include "stdafx.h"
#include "PickUpFactory.h"


PickUpFactory::PickUpFactory(SDL_Renderer* t_renderer):
	m_renderer(t_renderer)
{
}

void PickUpFactory::createPlayer(Entity& t_entity)
{
}

void PickUpFactory::createEnemy(int t_type, Entity& t_entity)
{
}

///<summary>
///Creating a Drop, pass in what enemy is desired. 1 - Ammo, 2 - Health and a reference to an entity. The required components and values will be added. 
///All values are set in the FactoryStatSheet.
///</summary>
void PickUpFactory::createDrop(int t_type, Entity& t_entity)
{
	t_entity.addComponent(new TransformComponent());
	t_entity.addComponent(new TagComponent(Tag::PickUp));
	t_entity.addComponent(new ColliderCircleComponent(FactoryStatSheet::HEALTH_RADIUS));
	t_entity.addComponent(new HealthComponent(1, 0));

	switch (t_type)
	{
	case 1:
		t_entity.addComponent(new VisualComponent("Ammobox_MachineGun.png", m_renderer));
		t_entity.addComponent(new PickUpComponent(PickupType::MachineGun));
		break;
	case 2:
		t_entity.addComponent(new VisualComponent("HealthBox.png", m_renderer));
		t_entity.addComponent(new PickUpComponent(PickupType::Health));
		break;
	case 3:
		t_entity.addComponent(new VisualComponent("AmmoBox_Grenade.png", m_renderer));
		t_entity.addComponent(new PickUpComponent(PickupType::Grenade));
		break;
	case 4:
		t_entity.addComponent(new VisualComponent("AmmoBox_Shotgun.png", m_renderer));
		t_entity.addComponent(new PickUpComponent(PickupType::Shotgun));
		break;
	default:
		t_entity.addComponent(new VisualComponent("HealthBox.png", m_renderer));
		t_entity.addComponent(new PickUpComponent(PickupType::Health));
		break;
	}
}
