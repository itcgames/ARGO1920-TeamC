#pragma once
#include <iostream>
#include <gtc/random.hpp>

#include "Debug.h"
#include "SDL.h"
#include "TransformComponent.h"
#include "VisualComponent.h"
#include "ColourComponent.h"
#include "Entity.h"
#include "BaseSystem.h"
#include "CompEnums.h"

class RenderSystem : public BaseSystem
{
public:
	~RenderSystem();
	void update(Entity& t_e);
	void render(SDL_Renderer* t_renderer, Entity& t_e);

	void renderPrimitives(SDL_Renderer* t_renderer, TransformComponent* t_posComp, ColourComponent* t_colourComp);

	//Renders texture at given point
	void renderTextures(VisualComponent* t_visComp, int t_x, int t_y, SDL_Renderer* t_gRenderer = NULL, SDL_Rect* t_clip = NULL, double t_angle = 0.0, SDL_Point* t_center = NULL, SDL_RendererFlip t_flip = SDL_FLIP_NONE);

private:

};
