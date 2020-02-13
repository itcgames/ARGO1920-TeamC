#pragma once

namespace Utilities
{
	const int SCREEN_WIDTH = 1920;
	const int SCREEN_HEIGHT = 1080;

	//font consts
	const int SMALL_FONT = 24;
	const int MEDIUM_FONT = 48;
	const int LARGE_FONT = 96;

	//audio consts
	const int AUDIO_FREQUENCY = 2 * MIX_DEFAULT_FREQUENCY;
	const int AUDIO_CHUNK_SIZE = 2048;

	const std::string IMAGES_PATH = "assets\\images\\";
	const std::string SOUNDS_PATH = "assets\\sounds\\";
	const std::string FONTS_PATH = "assets\\fonts\\";
}