#include "stdafx.h"
#include "PlayerFactory.h"

PlayerFactory::PlayerFactory()
{
}

///<summary>
///Setting the rendererer variable now that it isn't null.
///</summary>
void PlayerFactory::initialise(SDL_Renderer* t_renderer)
{
	m_renderer = t_renderer;
}

void PlayerFactory::createPlayer(Entity& t_entity)
{
}

///<summary>
///Creating a Player, pass in a reference to an entity. The required components and values will be added. 
///All values are set in the FactoryStatSheet.
///</summary>
void PlayerFactory::createPlayer(Entity& t_entity, bool t_isPlayer, Controller& t_controllers, int t_index, ButtonCommandMap t_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::S_MAX_PLAYERS])
{
	t_entity.addComponent(new CommandComponent());
	int playerHp = FactoryStatSheet::PLAYER_MAX_HP;
	if (Utilities::Achievements::numberOfUnlockedAchv == 1)
	{
		playerHp = FactoryStatSheet::PLAYER_MAX_HP_1;
	}
	else if (Utilities::Achievements::numberOfUnlockedAchv == 2)
	{
		playerHp = FactoryStatSheet::PLAYER_MAX_HP_2;
	}
	else if (Utilities::Achievements::numberOfUnlockedAchv == 3)
	{
		playerHp = FactoryStatSheet::PLAYER_MAX_HP_3;
	}
	const int PLAYER_MAX_HP = playerHp;
	t_entity.addComponent(new HealthComponent(PLAYER_MAX_HP, PLAYER_MAX_HP, FactoryStatSheet::PLAYER_INVINCIBILITY_FRAMES));
	t_entity.addComponent(new TransformComponent());
	t_entity.addComponent(new ForceComponent());
	t_entity.addComponent(new ColliderCircleComponent(Utilities::PLAYER_RADIUS));
	t_entity.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
	t_entity.addComponent(new PrimitiveComponent());
	t_entity.addComponent(new TagComponent(Tag::Player));
	t_entity.addComponent(new FireRateComponent(Utilities::PLAYER_FIRE_DELAY));
	t_entity.addComponent(new WeaponComponent());
	t_entity.addComponent(new FSMComponent());

	if (t_isPlayer)
	{
		t_entity.addComponent(new InputComponent(t_controllers,
			t_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)][t_index],
			t_controllerButtonMaps[static_cast<int>(ButtonState::Held)][t_index],
			t_controllerButtonMaps[static_cast<int>(ButtonState::Released)][t_index]));
			
	}
	else
	{
		t_entity.addComponent(new AiComponent(AITypes::ePlayerBot, AIStates::eWander, 0, 0));
	}

	switch (t_index)
	{
	case 0:
		t_entity.addComponent(new VisualComponent("Player_1.png", m_renderer, glm::vec2(10, 0)));
		break;
	case 1:
		t_entity.addComponent(new VisualComponent("Player_2.png", m_renderer, glm::vec2(10, 0)));
		break;
	case 2:
		t_entity.addComponent(new VisualComponent("Player_3.png", m_renderer, glm::vec2(10, 0)));
		break;
	case 3:
		t_entity.addComponent(new VisualComponent("Player_4.png", m_renderer, glm::vec2(10, 0)));
		break;
	default:
		break;
	}
}

void PlayerFactory::createEnemy(int t_type, Entity& t_entity)
{
}

void PlayerFactory::createDrop(int t_type, Entity& t_entity)
{
}
