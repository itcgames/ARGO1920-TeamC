#include "stdafx.h"
#include "AudioManager.h"

AudioManager* AudioManager::s_instance = NULL;

AudioManager* AudioManager::Instance()
{
	if (s_instance == NULL)
	{
		s_instance = new AudioManager();
	}
	return s_instance;
}

void AudioManager::Release()
{
	delete s_instance;
	s_instance = NULL;
}

/// <summary>
/// Plays a music file if already loaded in the asset manager, otherwise loads and plays.
/// </summary>
/// <param name="t_filename">File name of the music file to play</param>
/// <param name="t_loops">Amount of times to loop, loops infinitely (~65k times) if left blank</param>
void AudioManager::PlayMusic(std::string t_filename, const int t_loops) const
{
	//play music
	Mix_PlayMusic(m_assetMgr->GetMusic(t_filename), t_loops);
	//set the volume to match settings
	Mix_VolumeMusic(calcVolume(m_musicVolume));
}

/// <summary>
/// pauses music if currently playing
/// </summary>
void AudioManager::PauseMusic() const
{
	if (Mix_PlayingMusic() != 0)
	{
		Mix_PauseMusic();
	}
}

/// <summary>
/// resumes music if currently paused
/// </summary>
void AudioManager::ResumeMusic() const
{
	if (Mix_PausedMusic != 0)
	{
		Mix_ResumeMusic();
	}
}

/// <summary>
/// stops music
/// </summary>
void AudioManager::StopMusic() const
{
	Mix_HaltMusic();
}

/// <summary>
/// Plays a given Sound Effect. Loads and plays if not loaded already.
/// </summary>
/// <param name="t_filename">File name of the SFX file</param>
/// <param name="t_loops">amount of times to loop, doesn't loop if left blank</param>
/// <param name="t_channel">what channel to play on, plays on a first free channel when left blank</param>
void AudioManager::PlaySfx(std::string t_filename, const int t_loops, const int t_channel) const
{
	Mix_VolumeChunk(m_assetMgr->GetSfx(t_filename), getVolumeFromPercentage(m_sfxVolume));
	int channel = Mix_PlayChannel(t_channel, m_assetMgr->GetSfx(t_filename), t_loops);
	Mix_Volume(channel, getVolumeFromPercentage(m_masterVolume));
}

/// <summary>
/// Plays a given Sound Effect at a position, if not loaded, loads and plays
/// </summary>
/// <param name="t_filename">filename of SFX</param>
/// <param name="t_sfxPosition">position at which to play</param>
/// <param name="t_focusPosition">position of the camera</param>
/// <param name="t_loops">how many times to play the sound, plays once be default</param>
/// <param name="t_channel">which channel to play on, plays on first free channel by default</param>
void AudioManager::PlaySfxAtPosition(std::string t_filename, glm::vec2 t_sfxPosition, glm::vec2 t_focusPosition, const int t_loops, const int t_channel) const
{
	//play the sound
	PlaySfx(t_filename, t_loops, t_channel);

	//relative position vector used to calculate the angle
	glm::vec2 relVec = t_sfxPosition - t_focusPosition;
	float angle = glm::degrees(std::atan2(relVec.y, relVec.x)) + 90.0f;

	//passing in negative angle is meant to work fine but there seems 
	//to be a bug with SDL_mixer where sound doesn't play from the
	//correct speaker. I manually adjust it to be within 0-360
	while (angle < 0)
	{
		angle += 360.0f;
	}
	while (angle > 360)
	{
		angle -= 360.0f;
	}

#ifdef AUDIO_SYS_DEBUG
	printf("Sound produced at position: [%s, %s] at angle: %s\n", std::to_string(relVec.x).c_str(), std::to_string(relVec.y).c_str(), std::to_string(angle).c_str());
#endif // AUDIO_SYS_DEBUG

	//screen width is considered as max distance for audible sounds
	//calculate how far the sound is in percent
	float maxDistPercent = (glm::length(relVec) / Utilities::SCREEN_WIDTH);
	maxDistPercent *= 100.0f;
	//this distance has to be from 0 to 255 so we get percent of this
	float attuentionPercent = (255 / 100.0f);
	//and then get the actual percentage as a value between 0 and 255
	int dist = maxDistPercent * attuentionPercent;

	Mix_SetPosition(Mix_GroupNewer(-1), angle, dist);
}

///// <summary>
///// Create a SFX that fades over time
///// </summary>
///// <param name="t_filename">file name of sfx to play</param>
///// <param name="t_seconds">time in seconds</param>
///// <param name="t_loops">amount of loops, otherwise plays once</param>
///// <param name="t_channel">specific audio channel to play on, otherwise plays on first free channel</param>
//void AudioManager::FadeInSfx(std::string t_filename, const float t_seconds, const int t_loops, const int t_channel)
//{
//	const float milliSec = t_seconds * 1000;
//
//	int channel = t_channel;
//
//	if (channel == Utilities::AUDIO_ALL_CHANNELS)
//	{
//		channel = Mix_GroupAvailable(Utilities::AUDIO_ALL_CHANNELS);
//		if (channel == -1)
//		{
//			channel = Mix_GroupOldest(Utilities::AUDIO_ALL_CHANNELS);
//		}
//	}
//
//	Mix_VolumeChunk(m_assetMgr->GetSfx(t_filename), calcVolume(m_sfxVolume));
//	Mix_FadeInChannel(channel, m_assetMgr->GetSfx(t_filename), t_loops, milliSec);
//}
//
//void AudioManager::FadeOutSfx(std::string t_filename, const float t_seconds, const int t_channel)
//{
//	const float milliSec = t_seconds * 1000;
//
//	PlaySfx(t_filename, 0, t_channel);
//	Mix_VolumeChunk(m_assetMgr->GetSfx(t_filename), calcVolume(m_sfxVolume));
//	Mix_FadeOutChannel(Mix_GroupNewer(Utilities::AUDIO_ALL_CHANNELS), milliSec);
//}

/// <summary>
/// Pauses all sound effects
/// </summary>
void AudioManager::PauseSfx() const
{
	Mix_Pause(Utilities::AUDIO_ALL_CHANNELS);
}

/// <summary>
/// Resumes all SFX
/// </summary>
void AudioManager::ResumeSfx() const
{
	Mix_Resume(Utilities::AUDIO_ALL_CHANNELS);
}

/// <summary>
/// Stops all SFX
/// </summary>
void AudioManager::StopSfx() const
{
	Mix_HaltChannel(Utilities::AUDIO_ALL_CHANNELS);
}

/// <summary>
/// Plays a SFX on a channel that is meant for Player fire sfx
/// </summary>
/// <param name="t_filename">File name of the fire sound</param>
/// <param name="t_playerPosition">position of the player</param>
/// <param name="t_focusPosition">position of the camera/listener</param>
/// <param name="t_loops">how many times to play the sound</param>
void AudioManager::PlayPlayerFireSfx(std::string t_filename, glm::vec2 t_playerPosition, glm::vec2 t_focusPosition, const int t_loops)
{
	//check for a free channel in the player fire group
	int channel = Mix_GroupAvailable(Utilities::AUDIO_PLAYERS_FIRE_CHANNELS);

	//if no channel free, get the oldest sound so we can replace it
	if (channel == -1)
	{
		channel = Mix_GroupOldest(Utilities::AUDIO_PLAYERS_FIRE_CHANNELS);
	}

#ifdef AUDIO_SYS_DEBUG
	//std::printf("Playing Player Fire Sound %s on channel %s\n", t_filename.c_str(), std::to_string(channel).c_str());
	std::printf("Playing Player Fire Sound on channel %s\n", std::to_string(channel - Utilities::AUDIO_PLAYERS_FIRE_CHANNELS + 1).c_str());
#endif // AUDIO_SYS_DEBUG

	//then play the sound at a position
	PlaySfxAtPosition(t_filename, t_playerPosition, t_focusPosition, t_loops, channel);
}

/// <summary>
/// Set Master Volume to a percentage within 0 and 100
/// </summary>
/// <param name="t_percent">volume percentage</param>
void AudioManager::SetMasterVolume(const int t_percent)
{
	//clamp passed percentage between 0 and 100
	m_masterVolume = glm::clamp(t_percent, 0, 100);

	//set master volume to the percentage
	Mix_Volume(Utilities::AUDIO_ALL_CHANNELS, m_masterVolume * MIX_MAX_VOLUME / 100.0f);
	Mix_VolumeMusic(calcVolume(m_musicVolume));

#ifdef AUDIO_SYS_DEBUG
	outputVolumes();
#endif // AUDIO_SYS_DEBUG
}

/// <summary>
/// Returns the percentage of master volume
/// </summary>
/// <returns>current master volume</returns>
int AudioManager::GetMasterVolume() const
{
	return m_masterVolume;
}

/// <summary>
/// Set Music Volume to a percentage within 0 and 100, this volume is a percentage of master volume
/// Eg. Master Volume at 50 and Music at 50 results in actual music volume of 25%
/// </summary>
/// <param name="t_percent">volume percentage</param>
void AudioManager::SetMusicVolume(const const int t_percent)
{
	//clamp passed percentage between 0 and 100
	m_musicVolume = glm::clamp(t_percent, 0, 100);

	//divide MIX_MAX_VOLUME by 100 to get percentage
	Mix_VolumeMusic(calcVolume(m_musicVolume));
#ifdef AUDIO_SYS_DEBUG
	outputVolumes();
#endif // AUDIO_SYS_DEBUG
}

/// <summary>
/// Returns current Music Volume
/// </summary>
/// <returns>Music Volume as percentage</returns>
int AudioManager::GetMusicVolume() const
{
	return m_musicVolume;
}

/// <summary>
/// Set SFX Volume to a percentage within 0 and 100, this volume is a percentage of master volume.
/// This updates all currently playing sounds.
/// Eg. Master Volume at 50 and SFX at 50 results in actual sfx volume of 25%
/// </summary>
/// <param name="t_percent">volume percentage</param>
void AudioManager::SetSfxVolume(const int t_percent)
{
	//clamp passed percentage between 0 and 100
	m_sfxVolume = glm::clamp(t_percent, 0, 100);

	for (auto& sfx : m_assetMgr->GetSfxMap())
	{
		Mix_VolumeChunk(sfx.second, getVolumeFromPercentage(m_sfxVolume));
	}

#ifdef AUDIO_SYS_DEBUG
	outputVolumes();
#endif // AUDIO_SYS_DEBUG
}

/// <summary>
/// Returns current SFX Volume
/// </summary>
/// <returns>Music Volume as percentage</returns>
int AudioManager::GetSfxVolume() const
{
	return m_sfxVolume;
}

AudioManager::AudioManager()
{
	m_assetMgr = AssetManager::Instance();

	//Initialize SDL_mixer
	if (Mix_OpenAudio(Utilities::AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, Utilities::AUDIO_CHUNK_SIZE) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
	m_initialisedFlags = Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);

	Mix_AllocateChannels(Utilities::AUDIO_MIX_CHANNELS);
	Mix_GroupChannels(Utilities::AUDIO_PLAYERS_FIRE_CHANNELS, Utilities::AUDIO_MIX_CHANNELS - 1, Utilities::AUDIO_PLAYERS_FIRE_CHANNELS);

	SetMasterVolume(m_masterVolume);
}

AudioManager::~AudioManager()
{
	m_assetMgr = NULL;
	Mix_CloseAudio();

	for (int i = 0; i < m_initialisedFlags; i++)
	{
		Mix_Quit();
	}
}

/// <summary>
/// Calculates actual volume impacted by master volume
/// </summary>
/// <param name="t_volume">percentage</param>
/// <returns>volume after applying master volume</returns>
int AudioManager::calcVolume(const int t_volume) const
{
	return (t_volume * (m_masterVolume * MIX_MAX_VOLUME / 100.0f)) / 100.0f;
}

/// <summary>
/// Couts each volume and actual volume of Music and Sfx
/// </summary>
void AudioManager::outputVolumes() const
{
	std::cout << "Master volume:\t" << m_masterVolume << std::endl;
	std::cout << "Music volume:\t" << m_musicVolume << "\treal volume\t" << calcVolume(m_musicVolume) << std::endl;
	std::cout << "SFX volume:\t" << m_sfxVolume << "\treal volume\t" << calcVolume(m_sfxVolume) << std::endl;
}

/// <summary>
/// calculates percentage of the volume, used when calculating Mix_Chunk volume
/// </summary>
/// <param name="t_percentage">percentage of SFX volume</param>
/// <returns>volume from percentage</returns>
int AudioManager::getVolumeFromPercentage(const int t_percentage) const
{
	return (t_percentage * MIX_MAX_VOLUME / 100.0f);
}
