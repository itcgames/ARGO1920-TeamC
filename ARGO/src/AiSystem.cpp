#include "stdafx.h"
#include "AiSystem.h"

AiSystem::AiSystem(Entity(&t_players)[Utilities::S_MAX_PLAYERS], std::vector<Entity>& t_enemies, EventManager& t_eventManager) :
	m_players(t_players),
	m_enemies(t_enemies),
	m_eventManager(t_eventManager)
{
	m_behaviourTree.addChild(new RetreatBehaviour(&m_botEnemyData));
	m_behaviourTree.addChild(new HoldBehaviour(&m_botEnemyData));
	m_behaviourTree.addChild(new GetAmmoBehaviour(&m_botPickupData));
	m_behaviourTree.addChild(new MoveToGoalBehaviour(&m_botGoalData));
	m_behaviourTree.addChild(new MoveToLeaderBehaviour(&m_botLeaderData));
}

AiSystem::~AiSystem()
{
	BaseSystem::~BaseSystem();
}

void AiSystem::update(Entity& t_entity)
{
	TransformComponent* posComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
	AiComponent* aiComp = static_cast<AiComponent*>(t_entity.getComponent(ComponentType::Ai));

	//make sure that entity is not missing crucial components
	if (posComp && aiComp)
	{
		TransformComponent* posComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
		AiComponent* aiComp = static_cast<AiComponent*>(t_entity.getComponent(ComponentType::Ai));
		ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getComponent(ComponentType::Force));
		
		switch (aiComp->getType())
		{
		case AITypes::eMelee:
			meleeAI(posComp, aiComp, forceComp);
			break;
		case AITypes::eRanged:
			rangedAI(posComp, aiComp, forceComp);
			break;
		case AITypes::ePlayerBot:
			playerAI(t_entity);
			break;
		}
	}
}

void AiSystem::meleeAI(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent)
{
	//The Only Possible States Available to Melee Enemies. Use to limit Behaviours
	switch (t_aiComponent->getStates())
	{
	case AIStates::eSleeping:
		sleep(t_posComp, t_aiComponent, t_forceComponent);
		break;
	case AIStates::eWander:
		wander(t_posComp, t_aiComponent, t_forceComponent);
		break;
	}
}

void AiSystem::rangedAI(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent)
{
	//The Only Possible States Available to Ranged Enemies. Use to limit Behaviours
	switch (t_aiComponent->getStates())
	{
	case AIStates::eSleeping:
		sleep(t_posComp, t_aiComponent, t_forceComponent);
		break;
	case AIStates::eWander:
		wander(t_posComp, t_aiComponent, t_forceComponent);
		break;
	}
}

void AiSystem::playerAI(Entity& t_entity)
{
	playerMovementDecision(t_entity);
	playerShootingDecision(t_entity);
}

void AiSystem::playerMovementDecision(Entity& t_entity)
{
	//set up data
	glm::vec2 botPos = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform))->getPos();
	m_botEnemyData.nearbyEnemies = 0;
	setEnemyData(botPos);
	setClosestLeaderData(botPos);
	setClosestPickupData(botPos);
	setGoalData(botPos);
	//query behaviour tree
	m_behaviourTree.run(t_entity);
}

void AiSystem::playerShootingDecision(Entity& t_entity)
{
	if (m_botEnemyData.distance < CAN_SEE_ENEMY_DISTANCE * CAN_SEE_ENEMY_DISTANCE)
	{
		glm::vec2 playerPos = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform))->getPos();
		glm::vec2 enemyPos = static_cast<TransformComponent*>(m_botEnemyData.enemy->getComponent(ComponentType::Transform))->getPos();
		m_eventManager.emitEvent(createBulletEvent{ t_entity, glm::normalize(enemyPos - playerPos) , 32, 0 });
	}
}
void AiSystem::setEnemyData(glm::vec2 t_botPosition)
{
	m_botEnemyData.distance = CAN_SEE_ENEMY_DISTANCE * CAN_SEE_ENEMY_DISTANCE;
	for (auto& enemy : m_enemies)
	{
		if (enemy.getComponent(ComponentType::ColliderCircle))
		{
			TransformComponent* transCompEnemy = static_cast<TransformComponent*>(enemy.getComponent(ComponentType::Transform));
			if (transCompEnemy)
			{
				float newDistance = glm::distance2(t_botPosition, transCompEnemy->getPos());
				if (newDistance < m_botEnemyData.distance)
				{
					m_botEnemyData.nearbyEnemies++;
					m_botEnemyData.enemy = &enemy;
					m_botEnemyData.distance = newDistance;
				}
			}
		}
	}
}

void AiSystem::setClosestLeaderData(glm::vec2 t_botPosition)
{
	m_botLeaderData.entity = &m_players[0];
	TransformComponent* transCompPlayer = static_cast<TransformComponent*>(m_players[0].getComponent(ComponentType::Transform));
	m_botLeaderData.distance = glm::distance2(t_botPosition, transCompPlayer->getPos());
	for (auto& player : m_players) //if there are any players we instead make the goal one of the players
	{
		if (player.getComponent(ComponentType::Input))
		{
			transCompPlayer = static_cast<TransformComponent*>(player.getComponent(ComponentType::Transform));
			if (transCompPlayer)
			{
				float newDistance = glm::distance2(t_botPosition, transCompPlayer->getPos());
				if (newDistance < m_botLeaderData.distance)
				{
					m_botLeaderData.entity = &player;
					m_botLeaderData.distance = newDistance;
				}
			}
		}
	}
}

void AiSystem::setClosestPickupData(glm::vec2 t_botPosition)
{
}

void AiSystem::setGoalData(glm::vec2 t_botPosition)
{
	m_botGoalData.entity = &m_players[0];
}

void AiSystem::wander(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent)
{
	//Gives a number between 1 and 20, -10 change the range to -9 and 10 then divides it by 10 to give a range of -.9 and 1.
	float tempAdjuster = (((rand() % 20 + 1)));
	tempAdjuster = (tempAdjuster - 10) / 10;

	//Randomly changes current angle by the max which is scaled by the random scalar
	t_posComp->setRotation(t_posComp->getRotation() + (t_aiComponent->getMaxRotation() * tempAdjuster));
	//Generates a unit vector in the given angle.
	glm::vec2 tempVelocity = glm::vec2(glm::cos(t_posComp->getRotation() * M_PI / 180), glm::sin(t_posComp->getRotation() * M_PI / 180));
	//Scales the Unit Vector by the length of the max speed.
	tempVelocity *= glm::length(t_aiComponent->getMaxSpeed());
	//Updates Position
	t_forceComponent->addForce(tempVelocity);
	//t_posComp->setPos(t_posComp->getPos() + tempVelocity);
}

void AiSystem::sleep(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent)
{
	//Nothing will add code to awake unit if a target (i.e player) comes within range once discussions are had.
}