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
void AudioManager::PlayMusic(std::string t_filename, const int t_loops)
{
	//play music
	Mix_PlayMusic(m_assetMgr->GetMusic(t_filename), t_loops);
	//set the volume to match settings
	Mix_VolumeMusic(calcVolume(m_musicVolume));
}

/// <summary>
/// pauses music if currently playing
/// </summary>
void AudioManager::PauseMusic()
{
	if (Mix_PlayingMusic() != 0)
	{
		Mix_PauseMusic();
	}
}

/// <summary>
/// resumes music if currently paused
/// </summary>
void AudioManager::ResumeMusic()
{
	if (Mix_PausedMusic != 0)
	{
		Mix_ResumeMusic();
	}
}

/// <summary>
/// stops music
/// </summary>
void AudioManager::StopMusic()
{
	Mix_HaltMusic();
}

/// <summary>
/// Plays a given Sound Effect. Loads and plays if not loaded already.
/// </summary>
/// <param name="t_filename">File name of the SFX file</param>
/// <param name="t_loops">amount of times to loop, doesn't loop if left blank</param>
/// <param name="t_channel">what channel to play on, plays on a first free channel when left blank</param>
void AudioManager::PlaySfx(std::string t_filename, const int t_loops, const int t_channel)
{
	Mix_VolumeChunk(m_assetMgr->GetSfx(t_filename), getVolFromPercentage(m_sfxVolume));
	int channel = Mix_PlayChannel(t_channel, m_assetMgr->GetSfx(t_filename), t_loops);
	Mix_Volume(channel, getVolFromPercentage(m_masterVolume));
}

void AudioManager::PlaySfxAtPosition(std::string t_filename, glm::vec2 t_sfxPosition, glm::vec2 t_focusPosition, const int t_loops, const int t_channel)
{
	PlaySfx(t_filename, t_loops, t_channel);

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

	float maxDistPercent = (glm::length(relVec) / Utilities::SCREEN_WIDTH);
	maxDistPercent *= 100.0f;
	float attuentionPercent = (255 / 100.0f);
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

void AudioManager::PauseSfx()
{
	Mix_Pause(Utilities::AUDIO_ALL_CHANNELS);
}

void AudioManager::ResumeSfx()
{
	Mix_Resume(Utilities::AUDIO_ALL_CHANNELS);
}

void AudioManager::StopSfx()
{
	Mix_HaltChannel(Utilities::AUDIO_ALL_CHANNELS);
}

void AudioManager::PlayPlayerFireSfx(std::string t_filename, glm::vec2 t_playerPosition, glm::vec2 t_focusPosition, const int t_loops)
{
	int channel = Mix_GroupAvailable(Utilities::AUDIO_PLAYERS_FIRE_CHANNELS);

	if (channel == -1)
	{
		channel = Mix_GroupOldest(Utilities::AUDIO_PLAYERS_FIRE_CHANNELS);
	}

#ifdef AUDIO_SYS_DEBUG
	std::printf("Playing Player Fire Sound %s on channel %s\n", t_filename.c_str(), std::to_string(channel).c_str());
#endif // AUDIO_SYS_DEBUG

	PlaySfxAtPosition(t_filename, t_playerPosition, t_focusPosition, t_loops, channel);
}

void AudioManager::SetMasterVolume(const int t_percent)
{
	//clamp passed percentage between 0 and 100
	m_masterVolume = glm::clamp(t_percent, 0, 100);

	//set master volume to the percentage
	Mix_Volume(Utilities::AUDIO_ALL_CHANNELS, m_masterVolume * MIX_MAX_VOLUME / 100.0f);
	Mix_VolumeMusic(calcVolume(m_musicVolume));

#ifdef AUDIO_SYS_DEBUG
	coutVolumes();
#endif // AUDIO_SYS_DEBUG
}

int AudioManager::GetMasterVolume() const
{
	return m_masterVolume;
}

void AudioManager::SetMusicVolume(const const int t_percent)
{
	//clamp passed percentage between 0 and 100
	m_musicVolume = glm::clamp(t_percent, 0, 100);

	//divide MIX_MAX_VOLUME by 100 to get percentage
	Mix_VolumeMusic(calcVolume(m_musicVolume));
#ifdef AUDIO_SYS_DEBUG
	coutVolumes();
#endif // AUDIO_SYS_DEBUG
}

int AudioManager::GetMusicVolume() const
{
	return m_musicVolume;
}

void AudioManager::SetSfxVolume(const int t_percent)
{
	//clamp passed percentage between 0 and 100
	m_sfxVolume = glm::clamp(t_percent, 0, 100);

	for (auto& sfx : m_assetMgr->GetSfxMap())
	{
		Mix_VolumeChunk(sfx.second, getVolFromPercentage(m_sfxVolume));
	}

#ifdef AUDIO_SYS_DEBUG
	coutVolumes();
#endif // AUDIO_SYS_DEBUG
}

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
	m_inittedFlags = Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);

	Mix_AllocateChannels(Utilities::AUDIO_MIX_CHANNELS);
	Mix_GroupChannels(Utilities::AUDIO_PLAYERS_FIRE_CHANNELS, Utilities::AUDIO_MIX_CHANNELS - 1, Utilities::AUDIO_PLAYERS_FIRE_CHANNELS);

	SetMasterVolume(m_masterVolume);
}

AudioManager::~AudioManager()
{
	m_assetMgr = NULL;
	Mix_CloseAudio();

	for (int i = 0; i < m_inittedFlags; i++)
	{
		Mix_Quit();
	}
}

int AudioManager::calcVolume(const int& t_volume)
{
	return (t_volume * (m_masterVolume * MIX_MAX_VOLUME / 100.0f)) / 100.0f;
}

void AudioManager::coutVolumes()
{
	std::cout << "Master volume:\t" << m_masterVolume << std::endl;
	std::cout << "Music volume:\t" << m_musicVolume << "\treal volume\t" << calcVolume(m_musicVolume) << std::endl;
	std::cout << "SFX volume:\t" << m_sfxVolume << "\treal volume\t" << calcVolume(m_sfxVolume) << std::endl;
}

int AudioManager::getVolFromPercentage(const int& t_percentage)
{
	return (t_percentage * MIX_MAX_VOLUME / 100.0f);
}
