#pragma once
#include "AssetManager.h"
#include "Utilities.h"

class AudioManager
{
public:
	static AudioManager* Instance();
	static void Release();

	void PlayMusic(std::string t_filename, int t_loops = -1);
	void PauseMusic();
	void ResumeMusic();
	
	void PlaySfx(std::string t_filename, int t_loops = 0, int t_channel = -1);

	void SetVolume(int t_volume, int t_channel = Utilities::ALL_AUDIO_CHANNELS);
	int GetVolume(int t_channel = Utilities::ALL_AUDIO_CHANNELS) const;

private:
	static AudioManager* s_instance;
	AssetManager* m_assetMgr;

	AudioManager();
	~AudioManager();
};

