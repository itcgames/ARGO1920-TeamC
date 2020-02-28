#include "stdafx.h"
#include "AiSystem.h"

AiSystem::AiSystem(Entity(&t_players)[Utilities::S_MAX_PLAYERS], Entity(&t_enemies)[Utilities::ENEMY_POOL_SIZE], Entity(&t_pickups)[Utilities::PICKUP_POOL_SIZE], Entity& t_goal, EventManager& t_eventManager, LevelManager& t_levelManager) :
	m_players(t_players),
	m_enemies(t_enemies),
	m_pickups(t_pickups),
	m_goal(t_goal),
	m_eventManager(t_eventManager),
	m_levelmanager(t_levelManager),
	m_currentWaypoint(glm::linearRand(0,4))
{
	m_behaviourTree.addChild(new RetreatBehaviour(&m_botEnemyData, m_levelmanager));
	m_behaviourTree.addChild(new HoldBehaviour(&m_botEnemyData, m_levelmanager));
	m_behaviourTree.addChild(new GetAmmoBehaviour(&m_botPickupData, m_levelmanager));
	m_behaviourTree.addChild(new GetHealthBehaviour(&m_botHealthPickupData, m_levelmanager));
	m_behaviourTree.addChild(new MoveToGoalBehaviour(&m_botGoalData, m_levelmanager));
	m_behaviourTree.addChild(new MoveToLeaderBehaviour(&m_botLeaderData, m_levelmanager));

	m_waypoints[1].destination = glm::vec2(Utilities::TILE_SIZE * 55, Utilities::TILE_SIZE * 3);
	m_waypoints[2].destination = glm::vec2(Utilities::TILE_SIZE * 3, Utilities::TILE_SIZE * 37);
	m_waypoints[3].destination = glm::vec2(Utilities::TILE_SIZE * 20, Utilities::TILE_SIZE * 37);
	m_waypoints[4].destination = glm::vec2(Utilities::TILE_SIZE * 20, Utilities::TILE_SIZE * 20);
}

AiSystem::~AiSystem()
{
	BaseSystem::~BaseSystem();
}

void AiSystem::update(Entity& t_entity)
{
	TransformComponent* posComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
	AiComponent* aiComp = static_cast<AiComponent*>(t_entity.getComponent(ComponentType::Ai));
	ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getComponent(ComponentType::Force));

	m_waypoints[0].destination = static_cast<TransformComponent*>(m_goal.getComponent(ComponentType::Transform))->getPos();
	//make sure that entity is not missing crucial components
	if (posComp && aiComp && forceComp)
	{
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
	t_aiComponent->setState(AIStates::eWander);
	Entity* tile = m_levelmanager.findAtPosition(t_posComp->getPos());
	glm::vec2 seekPosition = glm::vec2(0, 0);
	if (tile)
	{
		FlowFieldComponent* flowFieldComp = static_cast<FlowFieldComponent*>(tile->getComponent(ComponentType::FlowField));
		if (flowFieldComp && flowFieldComp->getWeight() < Utilities::MAX_FLOW_FIELD_WEIGHT)
		{
			if (flowFieldComp->getWeight() == 0)
			{
				for (auto& player : m_players)
				{
					HealthComponent* healthComp = static_cast<HealthComponent*>(player.getComponent(ComponentType::Health));
					if (healthComp && healthComp->isAlive())
					{
						TransformComponent* transComp = static_cast<TransformComponent*>(player.getComponent(ComponentType::Transform));
						if (glm::distance2(t_posComp->getPos(), transComp->getPos()) < Utilities::TILE_SIZE * Utilities::TILE_SIZE)
						{
							seekPosition = transComp->getPos();
							t_aiComponent->setState(AIStates::eSeek);
						}
					}
				}
			}
			else if (flowFieldComp->getClosestNeightbour())
			{
				TransformComponent* transComp = static_cast<TransformComponent*>(flowFieldComp->getClosestNeightbour()->getComponent(ComponentType::Transform));
				if (transComp)
				{
					seekPosition = transComp->getPos() + glm::vec2(Utilities::TILE_SIZE / 2, Utilities::TILE_SIZE / 2);
					t_aiComponent->setState(AIStates::eSeek);
				}
			}
		}
	}

	//The Only Possible States Available to Melee Enemies. Use to limit Behaviours
	switch (t_aiComponent->getStates())
	{
	case AIStates::eSleeping:
		sleep(t_posComp, t_aiComponent, t_forceComponent);
		break;
	case AIStates::eWander:
		wander(t_posComp, t_aiComponent, t_forceComponent);
		break;
	case AIStates::eSeek:
		seek(t_posComp, t_aiComponent, t_forceComponent, seekPosition);
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

void AiSystem::seek(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent, glm::vec2 t_destination)
{
	glm::vec2 direction = glm::normalize(t_destination - t_posComp->getPos());
	t_forceComponent->addForce(direction);
	t_posComp->setRotation(glm::degrees(atan2(direction.y, direction.x)));
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
	setLeader();
	setCurrentWaypoint();
	m_botEnemyData.nearbyEnemies = 0;
	m_botPickupData.entity = nullptr;
	m_botHealthPickupData.entity = nullptr;
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
		TransformComponent* playerTransform = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
		glm::vec2 enemyPos = static_cast<TransformComponent*>(m_botEnemyData.enemy->getComponent(ComponentType::Transform))->getPos();
		Weapon weapon = static_cast<WeaponComponent*>(t_entity.getComponent(ComponentType::Weapon))->getCurrent();
		Controller temp;
		glm::vec2 fireDirection = glm::normalize(enemyPos - playerTransform->getPos());
		playerTransform->setRotation(glm::degrees(atan2(fireDirection.y, fireDirection.x)));
		m_eventManager.emitEvent(CreateBulletEvent{ t_entity, fireDirection , 32, weapon , temp });
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
			HealthComponent* healthCompEnemy = static_cast<HealthComponent*>(enemy.getComponent(ComponentType::Health));
			if (transCompEnemy && healthCompEnemy && healthCompEnemy->isAlive())
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
	m_botHealthPickupData.distance = CAN_SEE_ENEMY_DISTANCE * CAN_SEE_ENEMY_DISTANCE;
	m_botPickupData.distance = CAN_SEE_ENEMY_DISTANCE * CAN_SEE_ENEMY_DISTANCE;
	for (auto& pickup : m_pickups)
	{
		if (pickup.getComponent(ComponentType::ColliderCircle))
		{
			TransformComponent* transComp = static_cast<TransformComponent*>(pickup.getComponent(ComponentType::Transform));
			HealthComponent* healthComp = static_cast<HealthComponent*>(pickup.getComponent(ComponentType::Health));
			PickUpComponent* pickupComp = static_cast<PickUpComponent*>(pickup.getComponent(ComponentType::PickUp));
			if (transComp && healthComp && pickupComp && healthComp->isAlive())
			{
				float newDistance = glm::distance2(t_botPosition, transComp->getPos());
				switch (pickupComp->getPickupType())
				{
				case PickupType::Health:
					if (true)
					{
						if (newDistance < m_botHealthPickupData.distance)
						{
							m_botHealthPickupData.distance = newDistance;
							m_botHealthPickupData.entity = &pickup;
						}
					}
					break;
				case PickupType::MachineGun:
				case PickupType::Grenade:
					if (newDistance < m_botPickupData.distance)
					{
						m_botPickupData.distance = newDistance;
						m_botPickupData.entity = &pickup;
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

void AiSystem::setGoalData(glm::vec2 t_botPosition)
{
	m_botGoalData.destination = m_waypoints[m_currentWaypoint].destination;
}

void AiSystem::setLeader()
{
	bool playerFound = false;
	for (auto& player : m_players)
	{
		if (!static_cast<AiComponent*>(player.getComponent(ComponentType::Ai)))
		{
			playerFound = true;
		}
	}

	if (!playerFound)
	{
		for (auto& player : m_players)
		{
			AiComponent* aiComp = static_cast<AiComponent*>(player.getComponent(ComponentType::Ai));
			HealthComponent* hpComp = static_cast<HealthComponent*>(player.getComponent(ComponentType::Health));
			if (hpComp->isAlive())
			{
				aiComp->setIsLeaser(true);
				break;
			}
		}
	}
}

void AiSystem::setCurrentWaypoint()
{
	glm::vec2 leaderPos;
	for (auto& player : m_players)
	{
		AiComponent* aiComp = static_cast<AiComponent*>(player.getComponent(ComponentType::Ai));
		HealthComponent* hpComp = static_cast<HealthComponent*>(player.getComponent(ComponentType::Health));
		TransformComponent* transformComp = static_cast<TransformComponent*>(player.getComponent(ComponentType::Transform));
		if (aiComp && hpComp->isAlive() && aiComp->getIsleader())
		{
			leaderPos = transformComp->getPos();
		}
	}

	//if the leader is close enough to its waypoint it decides to get a new waypoint.
	if (glm::distance2(leaderPos, m_waypoints[m_currentWaypoint].destination) < Utilities::TILE_SIZE * Utilities::TILE_SIZE)
	{
		bool success = false;

		while (!success)
		{
			int newWayPoint = glm::linearRand(0, 4);
			if (newWayPoint != m_currentWaypoint)
			{
				m_currentWaypoint = newWayPoint;
				success = true;
			}
		}
	}
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
