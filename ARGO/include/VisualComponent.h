#pragma once
#include "Component.h"
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_ttf.h"
#include <string>

class VisualComponent : public Component
{
public:
	VisualComponent();
	~VisualComponent();

	//Loads image at specified path
	bool loadFromFile(std::string path, SDL_Renderer* gRenderer);

//#ifdef _SDL_TTF_H
	//Creates image from font string
	VisualComponent(std::string fontpath);
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Renderer* gRenderer);
	TTF_Font* gFont;
//#endif

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);


	//Gets image dimensions
	int getWidth() const;
	int getHeight() const;
	SDL_Texture* getTexture() const;

private:
	//The actual hardware texture
	SDL_Texture* m_texture;

	//Image dimensions
	int m_width;
	int m_height;
};

