#pragma once
#include <iostream>
#include <gtc/random.hpp>

#include "Debug.h"
#include "SDL.h"
#include "TransformComponent.h"
#include "VisualComponent.h"
#include "ColourComponent.h"
#include "TextComponent.h"
#include "Entity.h"
#include "BaseSystem.h"
#include "CompEnums.h"
#include "ParticleEmitterComponent.h"
#include "PrimitiveComponent.h"
#include "Utilities.h"

class RenderSystem : public BaseSystem
{
public:
	~RenderSystem();
	void update(Entity& t_entity);
	void render(SDL_Renderer* t_renderer, Entity& t_entity);
	void setFocus(glm::vec2 t_point);

private:
	void renderPrimitives(SDL_Renderer* t_renderer, TransformComponent* t_posComp, ColourComponent* t_colourComp);
	void renderParticles(SDL_Renderer* t_renderer, ParticleEmitterComponent* t_emitter, PrimitiveComponent* t_primitive, ColourComponent* t_colComp, TransformComponent* t_posComp);
	void renderTexture(VisualComponent* t_visComp, int t_textureLeftPos, int t_textureTopPos, SDL_Renderer* t_renderer = NULL, SDL_Rect* t_clip = NULL, double t_angle = 0.0, SDL_Point* t_center = NULL, SDL_RendererFlip t_flip = SDL_FLIP_NONE);
	void renderText(SDL_Renderer* t_renderer, TransformComponent* t_posComp, TextComponent* t_textComp);
	glm::vec2 m_focusPoint;
};

