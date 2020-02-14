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
}