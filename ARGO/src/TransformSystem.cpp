#include "stdafx.h"
#include "TransformSystem.h"

TransformSystem::~TransformSystem()
{
	BaseSystem::~BaseSystem();
}

void TransformSystem::update(Entity& t_e)
{
	TransformComponent* posComp = dynamic_cast<TransformComponent*>(t_e.getComponent(ComponentType::Transform));
	if (t_e.getComponent(ComponentType::Input) != nullptr)
	{
		InputComponent* inputComp = dynamic_cast<InputComponent*>(t_e.getComponent(ComponentType::Input));
		updateWithInput(posComp, inputComp);
	}
	checkBorder(posComp);
}

void TransformSystem::checkBorder(TransformComponent* t_pos)
{

	if (t_pos->getPos().x > 825)
	{
		t_pos->setX(-25);
	}
	else if (t_pos->getPos().x < -25)
	{
		t_pos->setX(825);
	}
	if (t_pos->getPos().y > 625)
	{
		t_pos->setY(-25);
	}
	else if (t_pos->getPos().y < -25)
	{
		t_pos->setY(625);
	}
}

void TransformSystem::updateWithInput(TransformComponent* t_pos, InputComponent* t_input)
{
	if (t_input != nullptr)
	{
		if (t_input->m_keys.up)
		{
			t_pos->moveUp();
		}
		if (t_input->m_keys.down)
		{
			t_pos->moveDown();
		}
		if (t_input->m_keys.left)
		{
			t_pos->moveLeft();
		}
		if (t_input->m_keys.right)
		{
			t_pos->moveRight();
		}
	}
}
