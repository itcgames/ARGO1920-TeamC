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
void PlayerFactory::createPlayer(Entity& t_entity, bool t_isPlayer, Controller& t_controllers, int t_index, ButtonCommandMap t_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS])
{
	t_entity.addComponent(new CommandComponent());
	t_entity.addComponent(new HealthComponent(FactoryStatSheet::PLAYER_MAX_HP, FactoryStatSheet::PLAYER_STARTING_HP, FactoryStatSheet::PLAYER_INVINCIBILITY_FRAMES));
	t_entity.addComponent(new TransformComponent());
	t_entity.addComponent(new ForceComponent());
	t_entity.addComponent(new ColliderCircleComponent(Utilities::PLAYER_RADIUS));
	t_entity.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
	t_entity.addComponent(new ParticleEmitterComponent(static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform))->getPos(), true,
		FactoryStatSheet::PARTICLE_DIRECTION_ANGLE_SAMPLE, FactoryStatSheet::PARTICLE_OFFSET_ANGLE_SAMPLE, FactoryStatSheet::PARTICLE_SPEED_SAMPLE,
		FactoryStatSheet::PARTICLE_MAX_PARTICLES_SAMPLE, FactoryStatSheet::PARTICLES_PER_SECOND_SAMPLE));
	t_entity.addComponent(new PrimitiveComponent());
	t_entity.addComponent(new TagComponent(Tag::Player));
	t_entity.addComponent(new VisualComponent("player.png", m_renderer, static_cast<Uint8>(glm::linearRand(0, 255)), static_cast<Uint8>(glm::linearRand(0, 255)), static_cast<Uint8>(glm::linearRand(0, 255))));
	t_entity.addComponent(new FireRateComponent(Utilities::PLAYER_FIRE_DELAY));

	if (t_isPlayer)
	{
		t_entity.addComponent(new InputComponent(t_controllers,
			t_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)],
			t_controllerButtonMaps[static_cast<int>(ButtonState::Held)],
			t_controllerButtonMaps[static_cast<int>(ButtonState::Released)]));
			
	}
	else
	{
		t_entity.addComponent(new AiComponent(AITypes::ePlayerBot, AIStates::eWander, 0, 0));
	}
}

void PlayerFactory::createEnemy(int t_type, Entity& t_entity)
{
}

void PlayerFactory::createDrop(int t_type, Entity& t_entity)
{
}
