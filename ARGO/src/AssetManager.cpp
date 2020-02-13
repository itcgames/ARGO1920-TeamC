#include "stdafx.h"
#include "AssetManager.h"

AssetManager* AssetManager::s_instance = NULL;

AssetManager* AssetManager::Instance(SDL_Renderer& t_renderer)
{
	if (s_instance == NULL)
	{
		s_instance = new AssetManager(t_renderer);
	}
	return s_instance;
}

AssetManager* AssetManager::Instance()
{
	return s_instance;
}

void AssetManager::Release()
{
	delete s_instance;
	s_instance = NULL;
}

SDL_Texture* AssetManager::GetTexture(std::string t_filename)
{
	std::string fullPath = SDL_GetBasePath();
	fullPath.append(Utilities::IMAGES_PATH + t_filename);

	if (!m_textures[fullPath])
	{
		m_textures.at(fullPath) = loadTexture(fullPath);
	}

	return m_textures[fullPath];
}

Mix_Chunk* AssetManager::GetSfx(std::string t_filename)
{
	std::string fullPath = SDL_GetBasePath();
	fullPath.append(Utilities::SOUNDS_PATH + t_filename);

	if (!m_sfx[fullPath])
	{
		m_sfx.at(fullPath) = loadSfx(fullPath);
	}

	return m_sfx[fullPath];
}

Mix_Music* AssetManager::GetMusic(std::string t_filename)
{
	std::string fullPath = SDL_GetBasePath();
	fullPath.append(Utilities::SOUNDS_PATH + t_filename);

	if (!m_music[fullPath])
	{
		m_music.at(fullPath) = loadMusic(fullPath);
	}

	return m_music[fullPath];
}

TTF_Font* AssetManager::GetFont(std::string t_filename, const int t_fontSize)
{
	std::string fullPath = SDL_GetBasePath();
	fullPath.append(Utilities::FONTS_PATH + t_filename);

	if (!m_fonts[fullPath])
	{
		m_fonts.at(fullPath) = loadFont(fullPath.c_str(), t_fontSize);
	}

	return m_fonts[fullPath];
}

AssetManager::AssetManager(SDL_Renderer& t_renderer) :
	m_renderer(t_renderer)
{
}

AssetManager::~AssetManager()
{
	//free up textures
	for (auto& tex : m_textures)
	{
		if (tex.second != NULL)
		{
			SDL_DestroyTexture(tex.second);
		}
	}
	m_textures.clear();

	//free up sfx 
	for (auto& sfx : m_sfx)
	{
		if (sfx.second != NULL)
		{
			Mix_FreeChunk(sfx.second);
		}
	}
	m_sfx.clear();

	//free up music
	for (auto& music : m_music)
	{
		if (music.second != NULL)
		{
			Mix_FreeMusic(music.second);
		}
	}
	m_music.clear();

	//free up music
	for (auto& font : m_fonts)
	{
		if (font.second != NULL)
		{
			TTF_CloseFont(font.second);
		}
	}
	m_fonts.clear();
}

SDL_Texture* AssetManager::loadTexture(std::string t_path)
{
	SDL_Texture* tex = NULL;

	SDL_Surface* surface = IMG_Load(t_path.c_str());

	if (surface == NULL)
	{
		printf("Image Load Error: Path(%s) - Error(%s)\n", t_path.c_str(), IMG_GetError());
		return tex;
	}

	tex = SDL_CreateTextureFromSurface(&m_renderer, surface);

	if (tex == NULL)
	{
		printf("Create Texture Error: %s\n", SDL_GetError());
		return tex;
	}

	SDL_FreeSurface(surface);

	return tex;
}

Mix_Music* AssetManager::loadMusic(std::string t_path)
{
	Mix_Music* music = NULL;

	music = Mix_LoadMUS(t_path.c_str());

	if (music == NULL)
	{
		printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
		return music;
	}
	return music;
}

Mix_Chunk* AssetManager::loadSfx(std::string t_path)
{
	Mix_Chunk* sfx = NULL;

	sfx = Mix_LoadWAV(t_path.c_str());

	if (sfx == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return sfx;
	}
	return sfx;
}

TTF_Font* AssetManager::loadFont(std::string t_path, const int t_fontSize)
{
	TTF_Font* font = NULL;

	font = TTF_OpenFont(t_path.c_str(), t_fontSize);

	if (font == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		return font;
	}
	return font;
}
