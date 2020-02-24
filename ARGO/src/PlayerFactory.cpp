#include "stdafx.h"
#include "PlayerFactory.h"
PlayerFactory::PlayerFactory(SDL_Renderer* t_renderer):
	m_renderer(t_renderer)
{
}

///<summary>
///Creating a Player, pass in a reference to an entity. The required components and values will be added. 
///All values are set in the FactoryStatSheet.
///</summary>
void PlayerFactory::createPlayer(Entity& t_entity)
{
	std::map<ButtonType, Command*> buttonPressMap = {
	std::pair<ButtonType, Command*>(ButtonType::DpadUp, new MoveUpCommand()),
	std::pair<ButtonType, Command*>(ButtonType::DpadDown, new MoveDownCommand()),
	std::pair<ButtonType, Command*>(ButtonType::DpadLeft, new MoveLeftCommand()),
	std::pair<ButtonType, Command*>(ButtonType::DpadRight, new MoveRightCommand()),
	std::pair<ButtonType, Command*>(ButtonType::RightTrigger, new FireBulletCommand()),
	std::pair<ButtonType,Command*>(ButtonType::Back, new CloseWindowCommand()) };

	t_entity.addComponent(new CommandComponent());
	t_entity.addComponent(new HealthComponent(FactoryStatSheet::PLAYER_MAX_HP, FactoryStatSheet::PLAYER_STARTING_HP));
	t_entity.addComponent(new TransformComponent());
	t_entity.addComponent(new InputComponent(buttonPressMap, buttonPressMap));
	t_entity.addComponent(new ForceComponent());
	t_entity.addComponent(new ColliderCircleComponent(Utilities::PLAYER_RADIUS));
	t_entity.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
	t_entity.addComponent(new ParticleEmitterComponent(static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform))->getPos(), true,
		FactoryStatSheet::PARTICLE_DIRECTION_ANGLE_SAMPLE, FactoryStatSheet::PARTICLE_OFFSET_ANGLE_SAMPLE, FactoryStatSheet::PARTICLE_SPEED_SAMPLE,
		FactoryStatSheet::PARTICLE_MAX_PARTICLES_SAMPLE, FactoryStatSheet::PARTICLES_PER_SECOND_SAMPLE));
	t_entity.addComponent(new PrimitiveComponent());
	t_entity.addComponent(new TagComponent(Tag::Player));
}

void PlayerFactory::createEnemy(int choice, Entity& t_entity)
{
}

void PlayerFactory::createDrop(int choice, Entity& t_entity)
{
}
