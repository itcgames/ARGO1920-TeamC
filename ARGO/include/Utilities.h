#pragma once


namespace Utilities
{

	class Achievements
	{
	public:
		static int numberOfUnlockedAchv;

	};

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
	const std::string BUG_DEATH_PATH = "BugDeath\\";
	const std::string MUSIC_PATH = "assets\\sounds\\music\\";
	const std::string FONTS_PATH = "assets\\fonts\\";
	const std::string FILES_PATH = "assets\\files\\";


#ifdef _DEBUG
	const int SCREEN_WIDTH = 1600;
	const int SCREEN_HEIGHT = 800;
#else
	const int SCREEN_WIDTH = 1920;
	const int SCREEN_HEIGHT = 1080;
#endif // _DEBUG

	const int LEVEL_TILE_WIDTH = 60;
	const int LEVEL_TILE_HEIGHT = 40;
	const int TILE_SIZE = 64;
	const int WALL_HEALTH = 5;

	const int PLAYER_RADIUS = 24;
	const int PLAYER_FIRE_DELAY = 1000 / 10;
	const float PLAYER_INVINCIBILITY_FRAMES = 32.0f;

	//Weapon Values
	const int NUMBER_OF_SHOTGUN_PELLETS = 5;


	const int ENEMY_RADIUS = 24;
	const int ENEMY_HP = 2;
	const int ENEMY_POOL_SIZE = 100;
	const float ENEMY_INITIAL_SPAWN_DELAY = 300;

	const glm::vec2 GOAL_START_POSITION = glm::vec2(51, 36) * (float)TILE_SIZE;

	const int NUMBER_OF_CONTROLLER_BUTTONS = 24;
 	const int NUMBER_OF_CONTROLLER_MAPS = 3;

	static const int PICKUP_POOL_SIZE = 20;

	//consts for audio
	const int AUDIO_ALL_CHANNELS = -1;
	const int AUDIO_GET_VOLUME = -1;
	const int AUDIO_INFINITE_LOOP = -1;
	const int AUDIO_MIX_CHANNELS = 128;
	const int AUDIO_VOLUME_STEP = 5;


	//channels per player for firing - amount TBD
	const int AUDIO_CHANNELS_PER_PLAYER = 8;
	const int AUDIO_PLAYERS_FIRE_CHANNELS = AUDIO_MIX_CHANNELS - AUDIO_CHANNELS_PER_PLAYER * 4;

 

	//Flow field values
	const int MAX_FLOW_FIELD_WEIGHT = 20;

	const float RADIANS_TO_DEGREES = 57.2958;
	static const int S_NUMBER_OF_SCREENS = 7; 

	static const glm::vec3 MENU_BUTTON_HIGHLIGHTED_COLOUR{ 255,255,240 };
	static const glm::vec3 MENU_BUTTON_DEFAULT_COLOUR{ 199,163,10 };
	static const glm::vec3 UI_COLOUR = glm::vec3(25, 57, 89);

	static const int GLOWSTICK_POOL_SIZE = 20;

	//HUD Values
	const float PERCENTAGE_BUFFER_BETWEEN_HUDS = 0.05f;//5% screen space.
	const float PERCENTAGE_SCREENSPACE_OF_HUD = 0.1875f;//18.75% screen space which is taken by each HUD.

	//Particles Manager
	//Base Particles
	//1-100 value
	const int PARTICLE_MAX_EMITTER_POOL = 30;
	const int PARTICLE_PERCENTAGE_SPEED_OFFSET = 50;
	const int PARTICLE_BASE_SPEED = 2;
	const int PARTICLE_MAX_PARTICLES = 100;
	const int PARTICLE_TIME_TO_KILL_PARTICLE = 20;
	const float PARTICLES_PER_SECOND = 25.0f;

	//Alien Death
	const glm::uvec4 PRIMARY_ENEMY_DEATH_COLOUR{ 0,50,0,255 };
	const glm::uvec4 SECONDARY_ENEMY_DEATH_COLOUR{ 0,200,0,255 };
	const float ENEMY_DEATH_MIN_PARTICLE_SIZE = 2.0f;
	const float ENEMY_DEATH_MAX_PARTICLE_SIZE = 10.0f;


	//Player Death
	const glm::uvec4 PRIMARY_PLAYER_DEATH_COLOUR{ 50,0,0,255 };
	const glm::uvec4 SECONDARY_PLAYER_DEATH_COLOUR{ 200,0,0,255 };
	const float PLAYER_DEATH_MIN_PARTICLE_SIZE = 2.0f;
	const float PLAYER_DEATH_MAX_PARTICLE_SIZE = 12.0f;

	//Explosion
	const glm::uvec4 PRIMARY_EXPLOSION_COLOUR{ 180,0,0,255 };
	const glm::uvec4 SECONDARY_EXPLOSION_COLOUR{ 255,255,0,255 };
	const float EXPLOSION_BASE_SPEED = 8.0f;
	const float EXPLOSION_MIN_PARTICLE_SIZE = 2.0f;
	const float EXPLOSION_MAX_PARTICLE_SIZE = 15.0f;
}

enum class MenuStates
{
	Game,
	MainMenu,
	Credits,
	Options,
	License,
	Splash,
	Achievements
};

enum class Systems
{
	AI,
	Collision,
	Command,
	Health,
	Input,
	Particle,
	Physics,
	Render
};

enum class MoveDirection
{
	Up,
	Down,
	Left,
	Right
}; 