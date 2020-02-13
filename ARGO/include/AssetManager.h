#pragma once
#include <SDL.h>
#include <map>
#include <Utilities.h>

class AssetManager
{
public:
	static AssetManager* Instance(SDL_Renderer& t_renderer);
	static AssetManager* Instance();
	static void Release();

	SDL_Texture* GetTexture(std::string t_filename);
	Mix_Chunk* GetSfx(std::string t_filename);
	Mix_Music* GetMusic(std::string t_filename);
	TTF_Font* GetFont(std::string t_filename, const int t_fontSize);

	AssetManager(SDL_Renderer& t_renderer);
	~AssetManager();


private:
	SDL_Texture* loadTexture(std::string t_path);
	Mix_Music* loadMusic(std::string t_path);
	Mix_Chunk* loadSfx(std::string t_path);
	TTF_Font* loadFont(std::string t_path, const int t_fontSize);

	static AssetManager* s_instance;
	std::map<std::string, SDL_Texture*> m_textures;
	std::map<std::string, Mix_Chunk*> m_sfx;
	std::map<std::string, Mix_Music*> m_music;
	std::map<std::string, TTF_Font*> m_fonts;

	SDL_Renderer& m_renderer;

};
