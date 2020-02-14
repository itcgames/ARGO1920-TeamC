#pragma once
#include "AssetManager.h"
#include "Utilities.h"

class AudioManager
{
public:
	static AudioManager* Instance();
	static void Release();

	void PlayMusic(std::string t_filename, const int t_loops = Utilities::AUDIO_INFINITE_LOOP);
	void PauseMusic();
	void ResumeMusic();

	void PlaySfx(std::string t_filename, const int t_loops = 0, const int t_channel = Utilities::ALL_AUDIO_CHANNELS);

	void SetMasterVolume(const int t_percent);
	int GetMasterVolume() const;

	void SetMusicVolume(const int t_percent);
	int GetMusicVolume() const;

	void SetSfxVolume(const int t_percent, const int t_channel = Utilities::ALL_AUDIO_CHANNELS);
	int GetSfxVolume() const;

private:
	static AudioManager* s_instance;
	AssetManager* m_assetMgr;

	AudioManager();
	~AudioManager();
	int calcVolume(const int& t_volume);

	int m_masterVolume = 50;
	int m_musicVolume = 100;
	int m_sfxVolume = 100;
};

