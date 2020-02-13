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
#include "Utilities.h"

class RenderSystem : public BaseSystem
{
public:
	~RenderSystem();
	void update(Entity& t_entity);
	void render(SDL_Renderer* t_renderer, Entity& t_entity);

	void renderPrimitives(SDL_Renderer* t_renderer, TransformComponent* t_posComp, ColourComponent* t_colourComp);
	void renderTextures(VisualComponent* t_visComp, int t_textureLeftPos, int t_textureTopPos, SDL_Renderer* t_renderer = NULL, SDL_Rect* t_clip = NULL, double t_angle = 0.0, SDL_Point* t_center = NULL, SDL_RendererFlip t_flip = SDL_FLIP_NONE);

	void setFocus(glm::vec2 t_point);
private:
	glm::vec2 m_focusPoint;
};

