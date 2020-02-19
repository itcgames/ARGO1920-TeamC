#pragma once

namespace Utilities
{
	const static int S_MAX_PLAYERS = 4;
	//font consts
	const int SMALL_FONT = 24;
	const int MEDIUM_FONT = 48;
	const int LARGE_FONT = 96;

	//audio consts
	const int AUDIO_FREQUENCY = 2 * MIX_DEFAULT_FREQUENCY;
	const int AUDIO_CHUNK_SIZE = 4096;

	const std::string IMAGES_PATH = "assets\\images\\";
	const std::string SFX_PATH = "assets\\sounds\\sfx\\";
	const std::string GUN_FIRE_PATH = "gunfire\\";
	const std::string MUSIC_PATH = "assets\\sounds\\music\\";
	const std::string FONTS_PATH = "assets\\fonts\\";

#ifdef _DEBUG
	const int SCREEN_WIDTH = 1600;
	const int SCREEN_HEIGHT = 800;
#else
	const int SCREEN_WIDTH = 1920;
	const int SCREEN_HEIGHT = 1080;
#endif // _DEBUG

	const float OFF_SCREEN_SCALAR = 0.75f;

	const int LEVEL_TILE_WIDTH = 60;
	const int LEVEL_TILE_HEIGHT = 40;
	const int TILE_SIZE = 64;
	const int WALL_HEALTH = 5;

	const int PLAYER_RADIUS = 32;
	const int PLAYER_FIRE_DELAY = 1000 / 10;

	const int ENEMY_RADIUS = 32;
	const int ENEMY_HP = 2;

	const int NUMBER_OF_CONTROLLER_BUTTONS = 16;

	//Particle
	const int PROJECTILE_SPEED = 32;
	const int PARTICLE_DIRECTION_ANGLE_SAMPLE = 90;
	const int PARTICLE_OFFSET_ANGLE_SAMPLE = 30;
	const int PARTICLE_SPEED_SAMPLE = 1;
	const int PARTICLE_MAX_PARTICLES_SAMPLE = 100;
	const int PARTICLES_PER_SECOND_SAMPLE = 20;

	//consts for audio
	const int AUDIO_ALL_CHANNELS = -1;
	const int AUDIO_GET_VOLUME = -1;
	const int AUDIO_INFINITE_LOOP = -1;
	const int AUDIO_MIX_CHANNELS = 128;
	const int AUDIO_VOLUME_STEP = 5;


	//channels per player for firing - amount TBD
	const int AUDIO_CHANNELS_PER_PLAYER = 8;
	const int AUDIO_PLAYERS_FIRE_CHANNELS = AUDIO_MIX_CHANNELS - AUDIO_CHANNELS_PER_PLAYER * 4;
}