#pragma once

namespace FactoryStatSheet
{

	//Player Starting Values
	const int PLAYER_MAX_HP = 10;
	const int PLAYER_STARTING_HP = 10;

	const int PARTICLE_DIRECTION_ANGLE_SAMPLE = 90;
	const int PARTICLE_OFFSET_ANGLE_SAMPLE = 30;
	const int PARTICLE_SPEED_SAMPLE = 1;
	const int PARTICLE_MAX_PARTICLES_SAMPLE = 100;
	const int PARTICLES_PER_SECOND_SAMPLE = 20;


	//Enemy Starting Values
	const float ENEMY_MAX_SPEED = 0.5f;
	const float ENEMY_TURN_RATE = 15.0f;
	const int ENEMY_RADIUS = 32;
	const int ENEMY_HP = 2;
	const int ENEMY_MAX_HP = 2;



	// Ammo Box Starting Values
	const glm::vec2 AMMO_POSITION(100, 100);
	const bool AMMO_ROTATING = true;
	const float AMMO_ROTATION_SPEED = 0.01f;
	const int AMMO_TYPE_ID = 1;
	const float AMMO_AMOUNT_CHANGE = 1;
	const int AMMO_RADIUS = 32;
	const int AMMO_HP = 1;


	//Health Box Startin Values
	const glm::vec2 HEALTH_POSITION(200, 200);
	const bool HEALTH_ROTATING = true;
	const float HEALTH_ROTATION_SPEED = 0.01f;
	const int HEALTH_TYPE_ID = 2;
	const float HEALTH_AMOUNT_CHANGE = 1;
	const int HEALTH_RADIUS = 32;
	const int HEALTH_HP = 1;

}