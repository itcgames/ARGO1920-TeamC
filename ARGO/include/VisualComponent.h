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
	VisualComponent(std::string t_filename, SDL_Renderer* t_renderer, glm::vec2 t_offset = glm::vec2(0,0), Uint8 t_red = 255, Uint8 t_green = 255, Uint8 t_blue = 255, bool t_staticPosition = false);
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

	void setStaticPosition(bool t_staticPosition);
	bool getStaticPosition();

	glm::vec2 getOffset();
	void setOffset(glm::vec2 t_offset);

private:
	//Pointer to a texture loaded from AssetManager
	SDL_Texture* m_texture;

	//Does the Sprite Move with the Camera. It doesn't by default
	bool m_staticPosition = false;

	//Image dimensions
	int m_width;
	int m_height;

	glm::vec2 m_offset;
};

