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

void AudioManager::PlayMusic(std::string t_filename, const int t_loops)
{
	Mix_PlayMusic(m_assetMgr->GetMusic(t_filename), t_loops);
	Mix_VolumeMusic(calcVolume(m_musicVolume));
}

void AudioManager::PauseMusic()
{
	if (Mix_PlayingMusic() != 0)
	{
		Mix_PauseMusic();
	}
}

void AudioManager::ResumeMusic()
{
	if (Mix_PausedMusic != 0)
	{
		Mix_ResumeMusic();
	}
}

void AudioManager::StopMusic()
{
	Mix_HaltMusic();
}

void AudioManager::PlaySfx(std::string t_filename, const int t_loops, const int t_channel)
{
	Mix_PlayChannel(t_channel, m_assetMgr->GetSfx(t_filename), t_loops);
	Mix_VolumeChunk(m_assetMgr->GetSfx(t_filename), calcVolume(m_sfxVolume));
}

void AudioManager::PlaySfxAtPosition(std::string t_filename, glm::vec2 t_sfxPosition, glm::vec2 t_focusPosition, const int t_loops, const int t_channel)
{
	Mix_PlayChannel(t_channel, m_assetMgr->GetSfx(t_filename), t_loops);
	Mix_VolumeChunk(m_assetMgr->GetSfx(t_filename), calcVolume(m_sfxVolume));
	
	glm::vec2 relVec = t_sfxPosition - t_focusPosition;
	float angle = glm::degrees(std::atan2(relVec.y, relVec.x)) + 90.0f;
	//return (t_volume * (m_masterVolume * MIX_MAX_VOLUME / 100.0f)) / 100.0f;

	float maxDistPercent = (glm::length(relVec) / Utilities::SCREEN_WIDTH);
	maxDistPercent *= 100.0f;
	float attuentionPercent = (255 / 100.0f);
	int dist = maxDistPercent * attuentionPercent;

	Mix_SetPosition(Mix_GroupNewer(-1), angle, dist);
}

void AudioManager::PauseSfx()
{
	Mix_Pause(Utilities::ALL_AUDIO_CHANNELS);
}

void AudioManager::ResumeSfx()
{
	Mix_Resume(Utilities::ALL_AUDIO_CHANNELS);
}

void AudioManager::StopSfx()
{
	Mix_HaltChannel(Utilities::ALL_AUDIO_CHANNELS);
}

void AudioManager::SetMasterVolume(const int t_percent)
{
	//clamp passed percentage between 0 and 100
	m_masterVolume = glm::clamp(t_percent, 0, 100);

	//set master volume to the percentage
	Mix_Volume(Utilities::ALL_AUDIO_CHANNELS, m_masterVolume * MIX_MAX_VOLUME / 100.0f);
	Mix_VolumeMusic(calcVolume(m_musicVolume));

	//coutVolumes();
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
}

int AudioManager::GetMusicVolume() const
{
	return m_musicVolume;
}

void AudioManager::SetSfxVolume(const int t_percent, const int t_channel)
{
	//clamp passed percentage between 0 and 100
	m_sfxVolume = glm::clamp(t_percent, 0, 100);

	//divide MIX_MAX_VOLUME by 100 to get percentage
	Mix_Volume(t_channel, calcVolume(m_sfxVolume));
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
	std::cout << "Master volume: " << m_masterVolume << std::endl;
	std::cout << "Music volume: " << m_musicVolume << " real volume " << calcVolume(m_musicVolume) << std::endl;
	std::cout << "SFX volume: " << m_sfxVolume << " real volume " << calcVolume(m_sfxVolume) << std::endl;
}
