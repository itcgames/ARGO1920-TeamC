#include "stdafx.h"
#include "EnemyFactory.h"

EnemyFactory::EnemyFactory()
{
}
///<summary>
///Setting the rendererer variable now that it isn't null.
///</summary>
void EnemyFactory::initialise(SDL_Renderer* t_renderer)
{
	m_renderer = t_renderer;
}

void EnemyFactory::createPlayer(Entity& t_entity)
{
}
///<summary>
///Creating an Enemy, pass in what enemy is desired. 1 - Melee, 2 - Ranged (WIP) and a reference to an entity. The required components and values will be added. 
///All values are set in the FactoryStatSheet.
///</summary>
void EnemyFactory::createEnemy(int choice, Entity& t_entity)
{
	switch (choice)
	{
	case 1:
		t_entity.addComponent(new TransformComponent());
		t_entity.addComponent(new ForceComponent());
		t_entity.addComponent(new AiComponent(AITypes::eMelee, AIStates::eWander, FactoryStatSheet::ENEMY_TURN_RATE, FactoryStatSheet::ENEMY_MAX_SPEED));
		t_entity.addComponent(new ColliderCircleComponent(FactoryStatSheet::ENEMY_RADIUS));
		t_entity.addComponent(new TagComponent(Tag::Enemy));
		t_entity.addComponent(new HealthComponent(FactoryStatSheet::ENEMY_MAX_HP, FactoryStatSheet::ENEMY_HP));
		t_entity.addComponent(new VisualComponent("EnemyFast.png", m_renderer));
		break;
	case 2:
		break;
	case 3:
		break;
	}
}

void EnemyFactory::createDrop(int choice, Entity& t_entity)
{
}
