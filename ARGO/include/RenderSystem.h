#pragma once
#include <iostream>
#include <gtc/random.hpp>

#include "Debug.h"
#include "SDL.h"
#include "TransformComponent.h"
#include "VisualComponent.h"
#include "ColourComponent.h"
#include "TextComponent.h"
#include "ColliderCircleComponent.h"
#include "Entity.h"
#include "BaseSystem.h"
#include "CompEnums.h"
#include "ParticleEmitterComponent.h"
#include "PrimitiveComponent.h"
#include "lightFieldComponent.h"
#include "FSMComponent.h"
#include "Utilities.h"
#include "HUDComponent.h"
#include "State.h"

class RenderSystem : public BaseSystem
{
public:
 	~RenderSystem();
	void update(Entity& t_entity);
	void render(SDL_Renderer* t_renderer, Entity& t_entity);
	void renderLight(SDL_Renderer* t_renderer, Entity& t_entity);
	void setFocus(glm::vec2 t_point);
	glm::vec2& getFocus();
	void operator=(const RenderSystem& t_otherRenderSystem);
	bool inView(TransformComponent* t_posComp);

private:
	void renderPrimitives(SDL_Renderer* t_renderer, TransformComponent* t_posComp, ColourComponent* t_colourComp, PrimitiveComponent* t_primitive);
	void renderParticles(SDL_Renderer* t_renderer, ParticleEmitterComponent* t_emitter, PrimitiveComponent* t_primitive, ColourComponent* t_colComp, TransformComponent* t_posComp);
	void renderTexture(VisualComponent* t_visComp, int t_textureLeftPos, int t_textureTopPos, SDL_Renderer* t_renderer = NULL, double t_angle = 0.0, SDL_Rect * t_clip = NULL, SDL_Point * t_center = NULL, SDL_RendererFlip t_flip = SDL_FLIP_NONE);
	void renderTexture(SDL_Texture* t_tex, int t_textureLeftPos, int t_textureTopPos, SDL_Renderer* t_renderer = NULL, double t_angle = 0.0, SDL_Rect* t_clip = NULL, SDL_Point* t_center = NULL, SDL_RendererFlip t_flip = SDL_FLIP_NONE);
	void renderText(SDL_Renderer* t_renderer, TransformComponent* t_posComp, TextComponent* t_textComp);
	void renderHUD(SDL_Renderer* t_renderer, TransformComponent* t_posComp, TextComponent* t_textComp, VisualComponent* t_visComp, PrimitiveComponent* t_primitiveComp, HUDComponent* t_hudComp);

	glm::vec2 m_focusPoint;

	const float OFF_SCREEN_SCALAR = 0.6f;
};

