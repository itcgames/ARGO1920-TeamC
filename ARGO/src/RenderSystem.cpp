#include "stdafx.h"
#include "RenderSystem.h"

RenderSystem::~RenderSystem()
{
	BaseSystem::~BaseSystem();
}

void RenderSystem::update(Entity& t_entity)
{
	//some sort of update to visual components here
	//maybe shaders/blending?
}

void RenderSystem::render(SDL_Renderer* t_renderer, Entity& t_entity)
{
	TransformComponent* posComp = dynamic_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
	VisualComponent* visComp = dynamic_cast<VisualComponent*>(t_entity.getComponent(ComponentType::Visual));

	if (visComp != nullptr)
	{
		renderTextures(visComp, posComp->getPos().x, posComp->getPos().y, t_renderer);
	}
	else
	{
		ColourComponent* colComp = dynamic_cast<ColourComponent*>(t_entity.getComponent(ComponentType::Colour));
		renderPrimitives(t_renderer, posComp, colComp);
	}

	if (t_entity.getAllComps().at(COMPONENT_ID::PARTICLE_ID) && t_entity.getAllComps().at(COMPONENT_ID::PRIMITIVE_ID))
	{
		ParticleEmitterComponent* emitComp = static_cast<ParticleEmitterComponent*>(t_entity.getAllComps().at(COMPONENT_ID::PARTICLE_ID));
		PrimitiveComponent* primComp = static_cast<PrimitiveComponent*>(t_entity.getAllComps().at(COMPONENT_ID::PRIMITIVE_ID));
		ColourComponent* colComp = dynamic_cast<ColourComponent*>(t_entity.getComponent(ComponentType::Colour));
		renderParticles(t_renderer, emitComp, primComp, colComp, posComp);
	}
}

void RenderSystem::renderPrimitives(SDL_Renderer* t_renderer, TransformComponent* t_posComp, ColourComponent* t_colComp)
{
	Uint8 prevRGBA[4];
	SDL_GetRenderDrawColor(t_renderer, &prevRGBA[0], &prevRGBA[1], &prevRGBA[2], &prevRGBA[3]);

	//hard-coded primitive size
	//fite me
	SDL_Rect rect;
	rect.x = t_posComp->getPos().x - 25;
	rect.y = t_posComp->getPos().y - 25;
	rect.w = 50;
	rect.h = 50;

	Colour colour;
	//set colour from the component
	if (t_colComp)
	{
		colour = t_colComp->getColour();
	}
	//else make it red
	else
	{
		colour.red = 255;
	}

	SDL_SetRenderDrawColor(t_renderer, colour.red, colour.green, colour.blue, colour.alpha);
	SDL_RenderFillRect(t_renderer, &rect);

	//reset the renderer to previous colour
	SDL_SetRenderDrawColor(t_renderer, prevRGBA[0], prevRGBA[1], prevRGBA[2], prevRGBA[3]);
}

void RenderSystem::renderParticles(SDL_Renderer* t_renderer, ParticleEmitterComponent* t_emitter, PrimitiveComponent* t_primitive, ColourComponent* t_colComp, TransformComponent* t_posComp)
{
	Uint8 prevRGBA[4];
	SDL_GetRenderDrawColor(t_renderer, &prevRGBA[0], &prevRGBA[1], &prevRGBA[2], &prevRGBA[3]);
	//hard-coded primitive size
	//fite me
	SDL_Rect rect;
	rect.w = t_primitive->getSize().x;
	rect.h = t_primitive->getSize().y;
	Colour colour;
	//set colour from the component
	if (t_colComp)
	{
		colour = t_colComp->getColour();
	}
	//else make it red
	else
	{
		colour.red = 255;
	}


	SDL_SetRenderDrawColor(t_renderer, colour.red, colour.green, colour.blue, colour.alpha);
	for (int i = 0; i < t_emitter->getMaxParticles(); i++)
	{
		
		if (t_emitter->getParticleAlive(i))
		{
			rect.x = t_emitter->getParticlePosition(i).x;
			rect.y = t_emitter->getParticlePosition(i).y;
			SDL_RenderFillRect(t_renderer, &rect);
		}
	}
	//reset the renderer to previous colour
	SDL_SetRenderDrawColor(t_renderer, prevRGBA[0], prevRGBA[1], prevRGBA[2], prevRGBA[3]);

}

void RenderSystem::renderTextures(VisualComponent* t_visComp, int t_textureLeftPos, int t_textureTopPos, SDL_Renderer* t_renderer, SDL_Rect* t_clip, double t_angle, SDL_Point* t_center, SDL_RendererFlip t_flip)
{
	SDL_Rect renderQuad = { t_textureLeftPos, t_textureTopPos, t_visComp->getWidth(), t_visComp->getHeight() };

	//Set clip rendering dimensions
	if (t_clip != NULL)
	{
		renderQuad.w = t_clip->w;
		renderQuad.h = t_clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(t_renderer, t_visComp->getTexture(), t_clip, &renderQuad, t_angle, t_center, t_flip);
}
