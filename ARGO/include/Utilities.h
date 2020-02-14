#pragma once

namespace Utilities
{
	//font consts
	const int SMALL_FONT = 24;
	const int MEDIUM_FONT = 48;
	const int LARGE_FONT = 96;

	//audio consts
	const int AUDIO_FREQUENCY = 2 * MIX_DEFAULT_FREQUENCY;
	const int AUDIO_CHUNK_SIZE = 4096;

	const std::string IMAGES_PATH = "assets\\images\\";
	const std::string SOUNDS_PATH = "assets\\sounds\\";
	const std::string FONTS_PATH = "assets\\fonts\\";

	const int SCREEN_WIDTH = 1600;
	const int SCREEN_HEIGHT = 800;
	const int LEVEL_TILE_WIDTH = 30;
	const int LEVEL_TILE_HEIGHT = 20;
	const int TILE_SIZE = 64;
	const int PLAYER_RADIUS = 32;
	const int ENEMY_RADIUS = 32;
	const int NUMBER_OF_CONTROLLER_BUTTONS = 16;
	const int PROJECTILE_SPEED = 32;

	//consts for audio
	const int ALL_AUDIO_CHANNELS = -1;
	const int GET_VOLUME = -1;
	const int AUDIO_INFINITE_LOOP = -1;
	const int AUDIO_MIX_CHANNELS = 64;
	const int PLAYER_1_FIRE_CHANNEL = 12;
	const int PLAYER_2_FIRE_CHANNEL = 13;
	const int PLAYER_3_FIRE_CHANNEL = 14;
	const int PLAYER_4_FIRE_CHANNEL = 15;
}