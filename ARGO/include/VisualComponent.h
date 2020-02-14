#pragma once
#include "Component.h"
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_ttf.h"
#include <string>
#include "AssetManager.h"

class VisualComponent : public Component
{
public:
	VisualComponent(std::string t_filename, SDL_Renderer* t_renderer);
	~VisualComponent();

	//Loads image at specified t_path
	bool loadFromFile(std::string t_filename, SDL_Renderer* t_renderer);

	//Set color modulation
	void setColor(Uint8 t_red, Uint8 t_green, Uint8 t_blue);

	//Set blend mode
	void setBlendMode(SDL_BlendMode t_blending);

	//Set t_alpha modulation
	void setAlpha(Uint8 t_alpha);

	//Gets image dimensions
	int getWidth() const;
	int getHeight() const;
	//get texture ptr
	SDL_Texture* getTexture() const;

private:
	//Pointer to a texture loaded from AssetManager
	SDL_Texture* m_texture;

	//Image dimensions
	int m_width;
	int m_height;
};

