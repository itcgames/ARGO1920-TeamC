#pragma once
#include "AssetManager.h"
#include "Utilities.h"

class AudioManager
{
private:
	int m_masterVolume = 50;
	int m_musicVolume = 20;
	int m_sfxVolume = 20;
	int m_initialisedFlags;

public:
	static AudioManager* Instance();
	static void Release();

	void PlayMusic(std::string t_filename, const int t_loops = Utilities::AUDIO_INFINITE_LOOP) const;
	void PauseMusic() const;
	void ResumeMusic() const;
	void StopMusic() const;

	void PlaySfx(std::string t_filename, const int t_loops = 0, const int t_channel = Utilities::AUDIO_ALL_CHANNELS) const;
	void PlaySfxAtPosition(std::string t_filename, glm::vec2 t_sfxPosition, glm::vec2 t_focusPosition, const int t_loops = 0, const int t_channel = Utilities::AUDIO_ALL_CHANNELS) const;

	//void FadeInSfx(std::string t_filename, const float t_seconds, const int t_loops = 0, const int t_channel = Utilities::AUDIO_ALL_CHANNELS);
	//void FadeOutSfx(std::string t_filename, const float t_seconds, const int t_channel = Utilities::AUDIO_ALL_CHANNELS);

	void PauseSfx() const;
	void ResumeSfx() const;
	void StopSfx() const;

	void PlayPlayerFireSfx(std::string t_filename, glm::vec2 t_playerPosition, glm::vec2 t_focusPosition, const int t_loops = 0);

	void SetMasterVolume(const int t_percent);
	int GetMasterVolume() const;

	void SetMusicVolume(const int t_percent);
	int GetMusicVolume() const;

	void SetSfxVolume(const int t_percent);
	int GetSfxVolume() const;

private:
	static AudioManager* s_instance;
	AssetManager* m_assetMgr;

	AudioManager();
	~AudioManager();
	int calcVolume(const int t_volume) const;
	void outputVolumes() const;
	int getVolumeFromPercentage(const int t_percentage) const;
};

