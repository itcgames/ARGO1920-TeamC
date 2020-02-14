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

void AudioManager::PlayMusic(std::string t_filename, int t_loops)
{
	Mix_PlayMusic(m_assetMgr->GetMusic(t_filename), t_loops);
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

void AudioManager::PlaySfx(std::string t_filename, int t_loops, int t_channel)
{
	Mix_PlayChannel(t_channel, m_assetMgr->GetSfx(t_filename) , t_loops);
	SetVolume(GetVolume(), t_channel);
}

void AudioManager::SetVolume(int t_volume, int t_channel)
{
	Mix_Volume(t_channel, glm::clamp(t_volume, 0, MIX_MAX_VOLUME));
}

int AudioManager::GetVolume(int t_channel) const
{
	return Mix_Volume(t_channel, -1);
}

AudioManager::AudioManager()
{
	m_assetMgr = AssetManager::Instance();

	//Initialize SDL_mixer
	if (Mix_OpenAudio(Utilities::AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, Utilities::AUDIO_CHUNK_SIZE) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
}

AudioManager::~AudioManager()
{
	m_assetMgr = NULL;
	Mix_Quit();
}